// Fill out your copyright notice in the Description page of Project Settings.


#include "DarknessComponent.h"

#include "EnhancedInputComponent.h"
#include "NiagaraComponent.h"
#include "PlayerStatsComponent.h"
#include "TopDownRPG/Player/RPGCharacter.h"


// Sets default values for this component's properties
UDarknessComponent::UDarknessComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	DarknessVFXComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DarknessVFX"));
}

void UDarknessComponent::SetupComponent(UPlayerSettings* Settings)
{
	Super::SetupComponent(Settings);

	if (UEnhancedInputComponent* Input = GetOwner()->GetComponentByClass<UEnhancedInputComponent>())
	{
		Input->BindAction(PlayerSettings->DarknessAction, ETriggerEvent::Started, this, &UDarknessComponent::OnDarkness);
	}

	StatsComponent = GetOwner()->GetComponentByClass<UPlayerStatsComponent>();
	if (ARPGCharacter* Character = Cast<ARPGCharacter>(GetOwner()))
	{
		PlayerHUD = Character->PlayerHUD;
		UpdateHUD();
	}
}

void UDarknessComponent::Dispose()
{
}

void UDarknessComponent::BeginPlay()
{
	DarknessVFXComponent->AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Super::BeginPlay();
}

void UDarknessComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                       FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (IsDark())
	{
		if (Darkness > 0)
		{
			Darkness -= PlayerSettings->DarknessUseRate * DeltaTime;
			if (Darkness <= 0)
			{
				Darkness = 0;
				SetDark(false);
			}

			UpdateHUD();
		}
	}
	
}

void UDarknessComponent::UpdateHUD()
{
	if (PlayerHUD)
	{
		PlayerHUD->SetDarkness(Darkness / MaxDarkness);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("DarknessComponent: PlayerHUD is not set!"));
	}
}

void UDarknessComponent::OnDarkness(const FInputActionValue& InputActionValue)
{
	if (Darkness >= MaxDarkness)
	{
		SetDark(true);
	}
}

void UDarknessComponent::AddDarkness(float Value)
{
	Darkness += Value;
	Darkness = FMath::Clamp(Darkness, 0.0f, MaxDarkness);
	UpdateHUD();
}

void UDarknessComponent::RemoveDarkness(float Value)
{
	Darkness -= Value;
	Darkness = FMath::Clamp(Darkness, 0.0f, MaxDarkness);
	UpdateHUD();
}

void UDarknessComponent::SetDark(bool bDark)
{
	if (bDark != bIsDark)
	{
		UE_LOG(LogTemp, Warning, TEXT("DarknessComponent: SetDark called with bDark = %s"), bDark ? TEXT("true") : TEXT("false"));
		bIsDark = bDark;

		if (bIsDark)
		{
			DarknessVFXComponent->Activate(true);
		}
		else
		{
			DarknessVFXComponent->Deactivate();
		}
	}
}

