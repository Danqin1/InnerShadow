// Fill out your copyright notice in the Description page of Project Settings.


#include "RageComponent.h"

#include "NiagaraComponent.h"
#include "PlayerStatsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TopDownRPG/Interfaces/PlayerInterface.h"
#include "TopDownRPG/Player/RPGCharacter.h"


// Sets default values for this component's properties
URageComponent::URageComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	RageVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DarknessVFX"));
}

void URageComponent::SetupComponent(UPlayerSettings* Settings)
{
	Super::SetupComponent(Settings);

	StatsComponent = GetOwner()->GetComponentByClass<UPlayerStatsComponent>();
	if (ARPGCharacter* Character = Cast<ARPGCharacter>(GetOwner()))
	{
		CharacterMovement = Character->GetCharacterMovement();
		DefaultMaxWalkSpeed = CharacterMovement ? CharacterMovement->MaxWalkSpeed : 0;
		PlayerHUD = Character->PlayerHUD;
		UpdateHUD();
	}
}

void URageComponent::Dispose()
{
}

void URageComponent::BeginPlay()
{
	RageVFXComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Super::BeginPlay();
}

void URageComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!StatsComponent)
	{
		return;
	}

	if (!IsInRage() && StatsComponent->GetEssence() >= StatsComponent->GetMaxEssence())
	{
		StartFrenzy();
	}

	if (IsInRage())
	{
		if (StatsComponent->GetEssence() > 0)
		{
			StatsComponent->RemoveEssence(PlayerSettings->DuringRageEssenceRemoveRate * DeltaTime);
			if (StatsComponent->GetEssence() <= 0)
			{
				FinishFrenzy();
			}

			UpdateHUD();
		}
	}
	
}

void URageComponent::StartFrenzy()
{
	if (bIsInRage || !StatsComponent)
	{
		return;
	}

	GetWorld()->GetTimerManager().ClearTimer(FrenzyRecoveryTimer);

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetOwner()))
	{
		if (PlayerInterface->GetState() == Dead)
		{
			return;
		}

		PlayerInterface->StopCurrentAnimation();
		PlayerInterface->ResetAttack();
		PlayerInterface->SetState(Nothing);
	}

	if (CharacterMovement)
	{
		CharacterMovement->StopMovementImmediately();
		CharacterMovement->MaxWalkSpeed = DefaultMaxWalkSpeed *
			(PlayerSettings ? PlayerSettings->FrenzyMovementControlMultiplier : 0.4f);
	}

	SetInRage(true);
}

void URageComponent::UpdateHUD()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetRage(IsInRage() ? GetRagePercent() : 0);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DarknessComponent: PlayerHUD is not set!"));
	}
}

float URageComponent::GetRagePercent() const
{
	return StatsComponent ? StatsComponent->GetEssencePercent() : 0;
}

void URageComponent::AddRage(float Value)
{
	if (StatsComponent)
	{
		StatsComponent->AddEssence(Value);
	}

	UpdateHUD();
}

void URageComponent::SetInRage(bool bDark)
{
	if (bDark != bIsInRage)
	{
		UE_LOG(LogTemp, Warning, TEXT("DarknessComponent: SetDark called with bDark = %s"), bDark ? TEXT("true") : TEXT("false"));
		bIsInRage = bDark;

		if (bIsInRage)
		{
			RageVFXComponent->Activate(true);
		}
		else
		{
			RageVFXComponent->Deactivate();
		}

		UpdateHUD();
	}
}

void URageComponent::FinishFrenzy()
{
	SetInRage(false);

	if (CharacterMovement)
	{
		CharacterMovement->StopMovementImmediately();
		CharacterMovement->MaxWalkSpeed = DefaultMaxWalkSpeed;
	}

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetOwner()))
	{
		PlayerInterface->SetState(Frozen);
		// TODO play frenzy recovery VFX
		GetWorld()->GetTimerManager().SetTimer(FrenzyRecoveryTimer, this, &URageComponent::RecoverFromFrenzy,
			PlayerSettings ? PlayerSettings->FrenzyRecoveryDuration : 0.75f, false);
	}
}

void URageComponent::RecoverFromFrenzy()
{
	if (CharacterMovement)
	{
		CharacterMovement->MaxWalkSpeed = DefaultMaxWalkSpeed;
	}

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetOwner()))
	{
		if (PlayerInterface->GetState() == Frozen)
		{
			PlayerInterface->ClearState(Frozen);
		}
	}
}

