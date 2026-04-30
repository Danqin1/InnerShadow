#include "UpgradeTrigger.h"

#include "Blueprint/UserWidget.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/Save/SaveSystem.h"
#include "TopDownRPG/UI/Progression/RunUpgradeChoiceUI.h"

AUpgradeTrigger::AUpgradeTrigger()
{
	PrimaryActorTick.bCanEverTick = false;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(SceneRoot);
	TriggerBox->SetBoxExtent(FVector(120.f, 120.f, 120.f));
	TriggerBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	TriggerBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	TriggerBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	TriggerBox->SetGenerateOverlapEvents(true);
	TriggerBox->SetCanEverAffectNavigation(false);
}

void AUpgradeTrigger::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &AUpgradeTrigger::HandleTriggerBeginOverlap);
	}

	if (TriggerSaveID.IsNone())
	{
		TriggerSaveID = GetFName();
	}

	if (IsTriggerAlreadyUsed())
	{
		DisableTrigger();
	}
}

void AUpgradeTrigger::HandleTriggerBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor || !OtherActor->IsA<ARPGCharacter>())
	{
		return;
	}

	if (IsTriggerAlreadyUsed())
	{
		DisableTrigger();
		return;
	}

	if (!TryShowUpgradeUI())
	{
		return;
	}

	if (USaveSystem* SaveSystem = GetGameInstance() ? GetGameInstance()->GetSubsystem<USaveSystem>() : nullptr)
	{
		SaveSystem->SetRunUpgradeTriggerTriggered(GetResolvedTriggerSaveID(), true);
	}

	DisableTrigger();
}

void AUpgradeTrigger::DisableTrigger()
{
	if (TriggerBox)
	{
		TriggerBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		TriggerBox->SetGenerateOverlapEvents(false);
	}

	SetActorEnableCollision(false);
	SetActorHiddenInGame(true);
}

FName AUpgradeTrigger::GetResolvedTriggerSaveID() const
{
	return TriggerSaveID.IsNone() ? GetFName() : TriggerSaveID;
}

bool AUpgradeTrigger::IsTriggerAlreadyUsed() const
{
	const UWorld* World = GetWorld();
	if (!World)
	{
		return false;
	}

	if (USaveSystem* SaveSystem = World->GetGameInstance() ? World->GetGameInstance()->GetSubsystem<USaveSystem>() : nullptr)
	{
		return SaveSystem->HasRunUpgradeTriggerBeenTriggered(GetResolvedTriggerSaveID());
	}

	return false;
}

bool AUpgradeTrigger::TryShowUpgradeUI()
{
	if (!RunUpgradeChoiceUIClass)
	{
		return false;
	}

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
	ActiveUpgradeUI = PlayerController
		? CreateWidget<URunUpgradeChoiceUI>(PlayerController, RunUpgradeChoiceUIClass)
		: CreateWidget<URunUpgradeChoiceUI>(GetWorld(), RunUpgradeChoiceUIClass);
	if (!ActiveUpgradeUI)
	{
		return false;
	}

	if (PlayerController)
	{
		ActiveUpgradeUI->AddToPlayerScreen();
	}
	else
	{
		ActiveUpgradeUI->AddToViewport();
	}

	return true;
}
