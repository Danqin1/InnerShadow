// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGCharacter.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RPGPlayerController.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "Materials/Material.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Perception/AISense_Hearing.h"
#include "Perception/AISense_Sight.h"
#include "TopDownRPG/DevDebug.h"
#include "TopDownRPG/Interfaces/DamageableInterface.h"

ARPGCharacter::ARPGCharacter()
{
	// Set size for player capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// Don't rotate character to camera direction
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // Rotate character to moving direction
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Activate ticking in order to update the cursor every frame.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	PlayerHUD = nullptr;

	PlayerStatsComponent = CreateDefaultSubobject<UPlayerStatsComponent>(TEXT("Player Stats"));
	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("Ability Component"));
	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory Component"));
	CombatComponent = CreateDefaultSubobject<UCombatComponent>(TEXT("Combat Component"));
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction Component"));
	StimulusSourceComponent = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("Stimulus Source"));
	DarknessVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DarknessVFX"));

	DarknessVFXComponent->SetupAttachment(RootComponent);
}

void ARPGCharacter::AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce)
{
	Super::AddMovementInput(WorldDirection, ScaleValue, bForce);
}

ECharacterState ARPGCharacter::GetState()
{
	return PlayerState;
}

void ARPGCharacter::SetState(ECharacterState NewState)
{
	if(NewState != PlayerState)
	{
		PlayerState = NewState;
		if(OnStateChanged.IsBound())
		{
			OnStateChanged.Broadcast(PlayerState);
		}
		PlayerHUD->StateChanged(PlayerState);

		if (PlayerState == Darkness)
		{
			DarknessVFXComponent->Activate(true);
		}
		else
		{
			DarknessVFXComponent->Deactivate();
		}
	}
}

void ARPGCharacter::ClearState(ECharacterState State)
{
	if(PlayerState == State)
	{
		SetState(Nothing);
	}
}

void ARPGCharacter::Damage(float Damage)
{
	if(PlayerStatsComponent)
	{
		PlayerStatsComponent->RemoveHP(Damage);
	}
	else
	{
		DevDebug::OnScreenLog("Stats component not exits");
	}
}

bool ARPGCharacter::CanDamage()
{
	return CombatComponent->CanDamage();
}

void ARPGCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDClass)
	{
		ARPGPlayerController* PC = GetController<ARPGPlayerController>();
		check(PC);
		PlayerHUD = CreateWidget<UPlayerHUD>(PC, PlayerHUDClass);
		check(PlayerHUD);
		PlayerHUD->AddToPlayerScreen();
	}

	TSet<UActorComponent*> Components = GetComponents();
	for (UActorComponent* Component : Components)
	{
		if (URPGActorComponentBase* RPGComponent = Cast<URPGActorComponentBase>(Component))
		{
			RPGComponent->SetupComponent(Settings);
		}
	}

	if(StimulusSourceComponent)
	{
		StimulusSourceComponent->RegisterForSense(TSubclassOf<UAISense_Sight>());
		StimulusSourceComponent->RegisterForSense(TSubclassOf<UAISense_Hearing>());
		StimulusSourceComponent->RegisterWithPerceptionSystem();
	}

	PlayerStatsComponent->OnDied.AddDynamic(this, &ARPGCharacter::Die);

	SetState(Nothing);

	StartLocation = GetActorLocation();
	StartRotation = GetActorRotation();
}

void ARPGCharacter::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (PlayerHUD)
	{
		PlayerHUD->RemoveFromParent();
		PlayerHUD = nullptr;
	}
	TSet<UActorComponent*> Components = GetComponents();
	for (UActorComponent* Component : Components)
	{
		if (URPGActorComponentBase* RPGComponent = Cast<URPGActorComponentBase>(Component))
		{
			RPGComponent->Dispose();
		}
	}

	Super::EndPlay(EndPlayReason);
}

void ARPGCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if(GetVelocity().Length() > 0.1)
	{
		UAISense_Hearing::ReportNoiseEvent(GetWorld(), GetActorLocation(), 1, this);
	}
}

void ARPGCharacter::Die()
{
	SetState(Dead);
	GetController()->DisableInput(UGameplayStatics::GetPlayerController(this, 0));
	
	FTimerHandle TimerHandle;
	FTimerDelegate Delegate;
	Delegate.BindUFunction(this, "Respawn");
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, Delegate, 3, false, 3);
}

void ARPGCharacter::Respawn()
{
	GetController()->EnableInput(UGameplayStatics::GetPlayerController(this, 0));
	PlayerStatsComponent->AddHP(100);
	SetActorLocation(StartLocation);
	SetActorRotation(StartRotation);
	SetState(Nothing);
}
