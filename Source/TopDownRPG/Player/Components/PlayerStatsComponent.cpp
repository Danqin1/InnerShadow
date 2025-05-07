// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatsComponent.h"

#include "TopDownRPG/Player/RPGCharacter.h"


// Sets default values for this component's properties
UPlayerStatsComponent::UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
	// ...
}

void UPlayerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	if (IICharacterState* CharacterState = Cast<IICharacterState>(GetOwner()))
	{
		if (CharacterState->GetState() == Dead)
		{
			return;
		}
		/*if (Darkness > 0)
		{
			Darkness -= (CharacterState->GetState() == ECharacterState::Darkness
				             ? PlayerSettings->DarknessRegen
				             : PlayerSettings->DarknessDefaultRegen) * DeltaTime;
			if (Darkness <= 0)
			{
				Darkness = 0;

				if (CharacterState->GetState() == ECharacterState::Darkness)
				{
					CharacterState->SetState(ECharacterState::Nothing);
				}
			}

			UpdateHUD();
		}*/
		if (HP < MaxHP && CharacterState->GetState() == ECharacterState::Nothing)
		{
			AddHP(PlayerSettings->HPRegen * DeltaTime);
		}
	}
}

void UPlayerStatsComponent::SetupComponent(UPlayerSettings* Settings)
{
	Super::SetupComponent(Settings);
	if (ARPGCharacter* Player = Cast<ARPGCharacter>(GetOwner()))
	{
		PlayerHUD = Player->PlayerHUD;
		check(PlayerHUD);
		PlayerHUD->SetHP(MaxHP);
	}

	UpdateHUD();
}

void UPlayerStatsComponent::Dispose()
{
}

void UPlayerStatsComponent::AddHP(float Value)
{
	
	HP = FMath::Clamp(HP + Value, 0, MaxHP);
	UpdateHUD();
}

void UPlayerStatsComponent::RemoveHP(float Value)
{
	if (IICharacterState* CharacterState = Cast<IICharacterState>(GetOwner()))
	{
		if (CharacterState->GetState() != ECharacterState::Dead)
		{
			HP = FMath::Clamp(HP - Value, 0, MaxHP);
			if (HP <= 0)
			{
				OnDied.Broadcast();
			}
			UpdateHUD();
		}
	}
}

void UPlayerStatsComponent::AddMaxHP(float Value)
{
	MaxHP += Value;
	HP = MaxHP;
	UpdateHUD();
}

void UPlayerStatsComponent::SetMaxHP(float Value)
{
	MaxHP = Value;
	HP = MaxHP;
	UpdateHUD();
}

void UPlayerStatsComponent::AddDarkness(float Value)
{
	/*if (Darkness + Value > MaxDarkness)
	{
		if (IICharacterState* CharacterState = Cast<IICharacterState>(GetOwner()))
		{
			if (CharacterState->GetState() != ECharacterState::Darkness)
			{
				CharacterState->SetState(ECharacterState::Darkness);
			}
		}
	}*/
	Darkness = FMath::Clamp(Darkness + Value, 0, MaxDarkness);
	UpdateHUD();
}

void UPlayerStatsComponent::RemoveDarkness(float Value)
{
	Darkness -= Value;
	UpdateHUD();
}

void UPlayerStatsComponent::AddPrize(FKillPrize KillPrize)
{
	AddDarkness(KillPrize.Darkness);
}

void UPlayerStatsComponent::UpdateHUD()
{
	if (!PlayerHUD)
	{
		return;
	}

	PlayerHUD->SetHP(HP / MaxHP);
	PlayerHUD->SetDarkness(Darkness / MaxDarkness);
}
