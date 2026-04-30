// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawner.h"

#include "InterchangeResult.h"
#include "Engine/CollisionProfile.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/Save/SaveSystem.h"
#include "TopDownRPG/UI/Wave/WaveUI.h"

// Sets default values
AWaveSpawner::AWaveSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	SpawnTrigger = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnTrigger"));
	SetRootComponent(SpawnTrigger);

	SpawnTrigger->SetBoxExtent(FVector(200.f, 200.f, 200.f));
	SpawnTrigger->SetCollisionProfileName(UCollisionProfile::Pawn_ProfileName);
	SpawnTrigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SpawnTrigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	SpawnTrigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	SpawnTrigger->SetGenerateOverlapEvents(true);

	WaveUIClass = UWaveUI::StaticClass();
}

// Called when the game starts or when spawned
void AWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (WaveSpawnerSaveID.IsNone())
	{
		WaveSpawnerSaveID = GetFName();
	}

	if (IsWaveSpawnerCompleted())
	{
		RestoreCompletedState();
		return;
	}

	if (SpawnTrigger)
	{
		SpawnTrigger->OnComponentBeginOverlap.AddDynamic(this, &AWaveSpawner::OnSpawnTriggerBeginOverlap);
	}
}

// Called every frame
void AWaveSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bSpawnSequenceStarted || SortedSpawnDelays.IsEmpty())
	{
		return;
	}

	SpawnSequenceElapsedTime += DeltaTime;

	while (NextSpawnDelayIndex < SortedSpawnDelays.Num() &&
		SpawnSequenceElapsedTime >= SortedSpawnDelays[NextSpawnDelayIndex])
	{
		const float Delay = SortedSpawnDelays[NextSpawnDelayIndex];
		++NextSpawnDelayIndex;
		ExecuteSpawnSet(Delay);
	}

	UpdateWaveUI();
	if (NextSpawnDelayIndex >= SortedSpawnDelays.Num())
	{
		FinishSpawnSequence();
	}
}

void AWaveSpawner::OnSpawnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!IsValid(OtherComp))
	{
		return;
	}

	if (IsWaveSpawnerCompleted())
	{
		RestoreCompletedState();
		return;
	}

	StartSpawnSequence();
}

void AWaveSpawner::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!IsValid(OtherActor) || OtherActor == this)
	{
		return;
	}

	if (IsWaveSpawnerCompleted())
	{
		RestoreCompletedState();
		return;
	}

	StartSpawnSequence();
}

void AWaveSpawner::StartSpawnSequence()
{
	if (bSpawnSequenceStarted || !IsValid(GetWorld()))
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("AWaveSpawner::StartSpawnSequence() Spawning WaveSpawner"));
	
	bSpawnSequenceStarted = true;
	SpawnSequenceElapsedTime = 0.f;
	NextSpawnDelayIndex = 0;

	if (SpawnTrigger)
	{
		SpawnTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	SpawnSetup.GetKeys(SortedSpawnDelays);
	SortedSpawnDelays.Sort();

	CreateWaveUI();
	UpdateWaveUI();

	if (SortedSpawnDelays.IsEmpty())
	{
		FinishSpawnSequence();
		return;
	}

	SetActorTickEnabled(true);
	
	if (StartGate)
	{
		StartGate->Close();
	}
	
	if (EndGate)
	{
		EndGate->Close();
	}
}

void AWaveSpawner::ExecuteSpawnSet(float Delay)
{
	const FSpawnSet* SpawnSet = SpawnSetup.Find(Delay);
	if (!SpawnSet || SpawnSet->EnemySpawners.IsEmpty())
	{
		return;
	}

	for (int32 Index = 0; Index < SpawnSet->SpawnCount; ++Index)
	{
		if (AEnemySpawner* EnemySpawner = SpawnSet->EnemySpawners[FMath::RandRange(0, SpawnSet->EnemySpawners.Num() - 1)])
		{
			EnemySpawner->Spawn();
		}
	}

	UpdateWaveUI();
}

void AWaveSpawner::FinishSpawnSequence()
{
	bSpawnSequenceStarted = false;
	SortedSpawnDelays.Reset();
	SetActorTickEnabled(false);
	RemoveWaveUI();
	
	if (StartGate)
	{
		StartGate->Open();
	}
	
	if (EndGate)
	{
		EndGate->Open();
	}

	if (USaveSystem* SaveSystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<USaveSystem>() : nullptr)
	{
		SaveSystem->SetWaveSpawnerCompleted(GetResolvedWaveSpawnerSaveID(), true);
	}
}

void AWaveSpawner::CreateWaveUI()
{
	if (WaveUI || !IsValid(GetWorld()))
	{
		return;
	}

	TSubclassOf<UWaveUI> WidgetClass = WaveUIClass;
	if (!WidgetClass)
	{
		WidgetClass = UWaveUI::StaticClass();
	}
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	WaveUI = PlayerController
		? CreateWidget<UWaveUI>(PlayerController, WidgetClass)
		: CreateWidget<UWaveUI>(GetWorld(), WidgetClass);

	if (WaveUI)
	{
		WaveUI->AddToViewport(20);
	}
}

void AWaveSpawner::RemoveWaveUI()
{
	if (WaveUI)
	{
		WaveUI->FadeOutAndDestroy();
		WaveUI = nullptr;
	}
}

void AWaveSpawner::UpdateWaveUI()
{
	if (!WaveUI)
	{
		return;
	}

	WaveUI->SetWavesLeft(GetRemainingWaves());

	if (NextSpawnDelayIndex < SortedSpawnDelays.Num())
	{
		const float NextDelay = SortedSpawnDelays[NextSpawnDelayIndex];
		const float PreviousDelay = NextSpawnDelayIndex > 0 ? SortedSpawnDelays[NextSpawnDelayIndex - 1] : 0.f;
		const float TotalDuration = FMath::Max(NextDelay - PreviousDelay, KINDA_SMALL_NUMBER);
		const float TimeRemaining = FMath::Max(NextDelay - SpawnSequenceElapsedTime, 0.f);

		WaveUI->SetNextWaveTimerVisible(true);
		WaveUI->SetNextWaveTimer(TimeRemaining, TotalDuration);
	}
	else
	{
		WaveUI->SetNextWaveTimerVisible(false);
	}
}

int32 AWaveSpawner::GetRemainingWaves() const
{
	return FMath::Max(SortedSpawnDelays.Num() - NextSpawnDelayIndex, 0);
}

FName AWaveSpawner::GetResolvedWaveSpawnerSaveID() const
{
	return WaveSpawnerSaveID.IsNone() ? GetFName() : WaveSpawnerSaveID;
}

bool AWaveSpawner::IsWaveSpawnerCompleted() const
{
	if (USaveSystem* SaveSystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<USaveSystem>() : nullptr)
	{
		return SaveSystem->IsWaveSpawnerCompleted(GetResolvedWaveSpawnerSaveID());
	}

	return false;
}

void AWaveSpawner::RestoreCompletedState()
{
	bSpawnSequenceStarted = false;
	SpawnSequenceElapsedTime = 0.f;
	NextSpawnDelayIndex = 0;
	SortedSpawnDelays.Reset();
	SetActorTickEnabled(false);
	RemoveWaveUI();

	if (SpawnTrigger)
	{
		SpawnTrigger->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		SpawnTrigger->SetGenerateOverlapEvents(false);
	}

	SetActorEnableCollision(false);

	if (StartGate)
	{
		StartGate->Open();
	}

	if (EndGate)
	{
		EndGate->Open();
	}
}

void AWaveSpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	RemoveWaveUI();

	Super::EndPlay(EndPlayReason);
}
