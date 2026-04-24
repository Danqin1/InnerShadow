// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveSpawner.h"

#include "InterchangeResult.h"
#include "Engine/CollisionProfile.h"

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
}

// Called when the game starts or when spawned
void AWaveSpawner::BeginPlay()
{
	Super::BeginPlay();

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

	StartSpawnSequence();
}

void AWaveSpawner::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!IsValid(OtherActor) || OtherActor == this)
	{
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
	if (!SpawnSet)
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
}

void AWaveSpawner::FinishSpawnSequence()
{
	SortedSpawnDelays.Reset();
	SetActorTickEnabled(false);
	
	if (StartGate)
	{
		StartGate->Open();
	}
	
	if (EndGate)
	{
		EndGate->Open();
	}
}
