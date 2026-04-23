// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerStatsComponent.h"

#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/Save/SaveSystem.h"

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
			AddHP(GetHealthRegenPerSecond() * DeltaTime);
		}

		if (Essence > 0 && CharacterState->GetState() == ECharacterState::Nothing && !CharacterState->IsInRage())
		{
			DurFromLastEssenceInput += DeltaTime;
			RemoveEssence(PlayerSettings->RageEssenceClearRate * DeltaTime * (FMath::Clamp(DurFromLastEssenceInput / 5, 0, 1) ));
		}
		else
		{
			DurFromLastEssenceInput = 0;
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
	}

	LoadPersistentStatsFromSave();
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
	PersistentStats.BonusMaxHP += Value;
	RecalculateDerivedStats();
	HP = MaxHP;
	UpdateHUD();
}

void UPlayerStatsComponent::SetMaxHP(float Value)
{
	BaseMaxHP = Value;
	RecalculateDerivedStats();
	HP = MaxHP;
	UpdateHUD();
}

void UPlayerStatsComponent::AddXP(int xp)
{
	XP += xp;
}

void UPlayerStatsComponent::AddEssence(float Value)
{
	Essence = FMath::Clamp(Essence + (Value * GetEssenceGainMultiplier()), 0, MaxEssence);
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
	const float BaseMultiplier = FMath::Max(0.0f, 1.0f + PersistentStats.DamageMultiplierBonus);
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetOwner()))
	{
		if (PlayerInterface->IsInRage())
		{
			return BaseMultiplier * PlayerSettings->FrenzyDamageMultiplier;
		}
	}

	switch (GetEssenceState())
	{
		case EDarkEssenceState::Empowered:
			return BaseMultiplier * PlayerSettings->EmpoweredDamageMultiplier;
		case EDarkEssenceState::Unstable:
			return BaseMultiplier * PlayerSettings->UnstableDamageMultiplier;
		case EDarkEssenceState::Frenzy:
			return BaseMultiplier * PlayerSettings->FrenzyDamageMultiplier;
		default:
			return BaseMultiplier;
	}
}

float UPlayerStatsComponent::GetIncomingDamageMultiplier() const
{
	const float BaseMultiplier = FMath::Max(0.1f, 1.0f + PersistentStats.IncomingDamageMultiplierBonus);
	switch (GetEssenceState())
	{
	case EDarkEssenceState::Unstable:
		return BaseMultiplier * PlayerSettings->UnstableIncomingDamageMultiplier;
	case EDarkEssenceState::Frenzy:
		return BaseMultiplier * PlayerSettings->FrenzyIncomingDamageMultiplier;
	default:
		return BaseMultiplier;
	}
}

float UPlayerStatsComponent::GetAttackSpeedMultiplier() const
{
	const float BaseMultiplier = FMath::Max(0.1f, 1.0f + PersistentStats.AttackSpeedMultiplierBonus);
	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(GetOwner()))
	{
		if (PlayerInterface->IsInRage())
		{
			return BaseMultiplier * PlayerSettings->FrenzyAttackSpeedMultiplier;
		}
	}

	switch (GetEssenceState())
	{
		case EDarkEssenceState::Empowered:
			return BaseMultiplier * PlayerSettings->EmpoweredAttackSpeedMultiplier;
		case EDarkEssenceState::Unstable:
			return BaseMultiplier * PlayerSettings->UnstableAttackSpeedMultiplier;
		case EDarkEssenceState::Frenzy:
			return BaseMultiplier * PlayerSettings->FrenzyAttackSpeedMultiplier;
		default:
			return BaseMultiplier;
	}
}

float UPlayerStatsComponent::GetHealthRegenPerSecond() const
{
	return FMath::Max(0.0f, PlayerSettings ? PlayerSettings->HPRegen + PersistentStats.BonusHPRegen : PersistentStats.BonusHPRegen);
}

float UPlayerStatsComponent::GetEssenceGainMultiplier() const
{
	return FMath::Max(0.0f, 1.0f + PersistentStats.EssenceGainMultiplierBonus);
}

void UPlayerStatsComponent::SetPersistentStats(const FPlayerPersistentStats& InStats)
{
	PersistentStats = InStats;
	RecalculateDerivedStats();
	UpdateHUD();
}

void UPlayerStatsComponent::RecalculateDerivedStats()
{
	const float OldMaxHP = MaxHP;
	MaxHP = FMath::Max(1.0f, BaseMaxHP + PersistentStats.BonusMaxHP);
	MaxEssence = FMath::Max(1.0f, BaseMaxEssence);

	if (OldMaxHP <= 0.0f)
	{
		HP = MaxHP;
	}
	else if (MaxHP > OldMaxHP)
	{
		HP += MaxHP - OldMaxHP;
	}

	HP = FMath::Clamp(HP, 0.0f, MaxHP);
	Essence = FMath::Clamp(Essence, 0.0f, MaxEssence);
}

void UPlayerStatsComponent::LoadPersistentStatsFromSave()
{
	RecalculateDerivedStats();

	if (UWorld* World = GetWorld())
	{
		if (UGameInstance* GameInstance = World->GetGameInstance())
		{
			if (USaveSystem* SaveSystem = GameInstance->GetSubsystem<USaveSystem>())
			{
				if (USaveData* SaveData = SaveSystem->GetSaveData())
				{
					PersistentStats = SaveData->PlayerStats;
				}
			}
		}
	}

	RecalculateDerivedStats();
	if (HP <= 0.0f)
	{
		HP = MaxHP;
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
