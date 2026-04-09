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

		if (Essence > 0 && CharacterState->GetState() == ECharacterState::Nothing && !CharacterState->IsInRage())
		{
			RemoveEssence(PlayerSettings->RageEssenceClearRate * DeltaTime);
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
		HP = MaxHP;
		PlayerHUD->SetHP(HP / MaxHP);
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

float UPlayerStatsComponent::GetEssencePercent() const
{
	return MaxEssence <= 0 ? 0 : Essence / MaxEssence;
}

EDarkEssenceState UPlayerStatsComponent::GetEssenceState() const
{
	if (Essence >= MaxEssence)
	{
		return EDarkEssenceState::Frenzy;
	}

	const float EssencePercent = GetEssencePercent();
	if (EssencePercent >= PlayerSettings->UnstableEssenceThreshold)
	{
		return EDarkEssenceState::Unstable;
	}

	if (EssencePercent >= PlayerSettings->EmpoweredEssenceThreshold)
	{
		return EDarkEssenceState::Empowered;
	}

	return EDarkEssenceState::Normal;
}

float UPlayerStatsComponent::GetOutgoingDamageMultiplier() const
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetOwner()))
	{
		if (PlayerInterface->IsInRage())
		{
			return PlayerSettings->FrenzyDamageMultiplier;
		}
	}

	switch (GetEssenceState())
	{
		case EDarkEssenceState::Empowered:
			return PlayerSettings->EmpoweredDamageMultiplier;
		case EDarkEssenceState::Unstable:
			return PlayerSettings->UnstableDamageMultiplier;
		case EDarkEssenceState::Frenzy:
			return PlayerSettings->FrenzyDamageMultiplier;
		default:
			return 1.0f;
	}
}

float UPlayerStatsComponent::GetIncomingDamageMultiplier() const
{
	switch (GetEssenceState())
	{
	case EDarkEssenceState::Unstable:
		return PlayerSettings->UnstableIncomingDamageMultiplier;
	case EDarkEssenceState::Frenzy:
		return PlayerSettings->FrenzyIncomingDamageMultiplier;
	default:
		return 1.0f;
	}
}

float UPlayerStatsComponent::GetAttackSpeedMultiplier() const
{
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetOwner()))
	{
		if (PlayerInterface->IsInRage())
		{
			return PlayerSettings->FrenzyAttackSpeedMultiplier;
		}
	}

	switch (GetEssenceState())
	{
		case EDarkEssenceState::Empowered:
			return PlayerSettings->EmpoweredAttackSpeedMultiplier;
		case EDarkEssenceState::Unstable:
			return PlayerSettings->UnstableAttackSpeedMultiplier;
		case EDarkEssenceState::Frenzy:
			return PlayerSettings->FrenzyAttackSpeedMultiplier;
		default:
			return 1.0f;
	}
}

void UPlayerStatsComponent::UpdateHUD()
{
	if (!PlayerHUD)
	{
		return;
	}

	PlayerHUD->SetHP(HP / MaxHP);
	PlayerHUD->SetEssence(GetEssencePercent());
}
