// Fill out your copyright notice in the Description page of Project Settings.


#include "RageComponent.h"

#include "EnhancedInputComponent.h"
#include "NiagaraComponent.h"
#include "PlayerStatsComponent.h"
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

	if (UEnhancedInputComponent* Input = GetOwner()->GetComponentByClass<UEnhancedInputComponent>())
	{
		Input->BindAction(PlayerSettings->RageAction, ETriggerEvent::Started, this, &URageComponent::OnRage);
	}

	StatsComponent = GetOwner()->GetComponentByClass<UPlayerStatsComponent>();
	if (ARPGCharacter* Character = Cast<ARPGCharacter>(GetOwner()))
	{
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

	if (IsInRage())
	{
		if (Rage > 0)
		{
			Rage -= PlayerSettings->DarknessUseRate * DeltaTime;
			if (Rage <= 0)
			{
				Rage = 0;
				SetInRage(false);
			}

			UpdateHUD();
		}
	}
	
}

void URageComponent::UpdateHUD()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetRage(Rage / MaxRage);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DarknessComponent: PlayerHUD is not set!"));
	}
}

void URageComponent::OnRage(const FInputActionValue& InputActionValue)
{
	if (Rage >= MaxRage)
	{
		SetInRage(true);
	}
}

void URageComponent::AddRage(float Value)
{
	Rage += Value;
	Rage = FMath::Clamp(Rage, 0.0f, MaxRage);
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
	}
}

