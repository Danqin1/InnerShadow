// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatsComponent.h"

#include "TopDownRPG/Player/RPGCharacter.h"

UPlayerStatsComponent::UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;
}

void UPlayerStatsComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	if (IPlayerInterface* CharacterState = Cast<IPlayerInterface>(GetOwner()))
	{
		if (CharacterState->GetState() == Dead)
		{
			return;
		}
		
		if (HP < MaxHP && CharacterState->GetState() == ECharacterState::Nothing)
		{
			AddHP(PlayerSettings->HPRegen * DeltaTime);
		}

		if (Essence < MaxEssence && CharacterState->GetState() == ECharacterState::Nothing)
		{
			AddEssence(PlayerSettings->EssenceRegen * DeltaTime);
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
	if (IPlayerInterface* CharacterState = Cast<IPlayerInterface>(GetOwner()))
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

void UPlayerStatsComponent::AddXP(int xp)
{
	XP += xp;
}

void UPlayerStatsComponent::AddEssence(float Value)
{
	Essence = FMath::Clamp(Essence + Value, 0, MaxEssence);
	UpdateHUD();
}

void UPlayerStatsComponent::RemoveEssence(float Value)
{
	Essence = FMath::Clamp(Essence - Value, 0, MaxEssence);
	UpdateHUD();
}

void UPlayerStatsComponent::UpdateHUD()
{
	if (!PlayerHUD)
	{
		return;
	}

	PlayerHUD->SetHP(HP / MaxHP);
	PlayerHUD->SetEssence(Essence / MaxEssence);
}
