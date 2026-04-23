#include "RunUpgradeSystem.h"

#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/Core/RPGGameInstance.h"
#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/Player/Components/PlayerStatsComponent.h"
#include "TopDownRPG/Save/SaveData.h"
#include "TopDownRPG/Save/SaveSystem.h"
#include "TopDownRPG/Progression/UpgradesDatabase.h"

void URunUpgradeSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (const URPGGameInstance* GameInstance = Cast<URPGGameInstance>(GetGameInstance()))
	{
		UpgradesDatabase = GameInstance->UpgradesDatabase;
	}
}

void URunUpgradeSystem::RestoreFromSave(USaveData* InSaveData)
{
	SaveData = InSaveData;
}

TArray<FRunUpgradeData> URunUpgradeSystem::GetRandomUpgradeChoices(int32 Count) const
{
	TArray<FRunUpgradeData> AvailableUpgrades;
	if (!UpgradesDatabase)
	{
		return AvailableUpgrades;
	}

	for (const FRunUpgradeData& Upgrade : UpgradesDatabase->Upgrades)
	{
		if (!SaveData || !SaveData->TakenUpgradeIDs.Contains(Upgrade.ID))
		{
			AvailableUpgrades.Add(Upgrade);
		}
	}

	TArray<FRunUpgradeData> Choices;
	const int32 MaxChoices = FMath::Clamp(Count, 0, AvailableUpgrades.Num());
	for (int32 Index = 0; Index < MaxChoices; ++Index)
	{
		const int32 RandomIndex = FMath::RandRange(0, AvailableUpgrades.Num() - 1);
		Choices.Add(AvailableUpgrades[RandomIndex]);
		AvailableUpgrades.RemoveAtSwap(RandomIndex);
	}

	return Choices;
}

bool URunUpgradeSystem::ApplyUpgradeByID(FName UpgradeID)
{
	if (!SaveData || SaveData->TakenUpgradeIDs.Contains(UpgradeID))
	{
		return false;
	}

	if (!UpgradesDatabase)
	{
		return false;
	}

	const FRunUpgradeData* Upgrade = UpgradesDatabase->GetUpgradeByID(UpgradeID);
	if (!Upgrade)
	{
		return false;
	}

	SaveData->PlayerStats.BonusMaxHP += Upgrade->StatModifiers.BonusMaxHP;
	SaveData->PlayerStats.BonusHPRegen += Upgrade->StatModifiers.BonusHPRegen;
	SaveData->PlayerStats.DamageMultiplierBonus += Upgrade->StatModifiers.DamageMultiplierBonus;
	SaveData->PlayerStats.IncomingDamageMultiplierBonus += Upgrade->StatModifiers.IncomingDamageMultiplierBonus;
	SaveData->PlayerStats.AttackSpeedMultiplierBonus += Upgrade->StatModifiers.AttackSpeedMultiplierBonus;
	SaveData->PlayerStats.EssenceGainMultiplierBonus += Upgrade->StatModifiers.EssenceGainMultiplierBonus;
	SaveData->TakenUpgradeIDs.Add(UpgradeID);

	if (ARPGCharacter* Character = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0)))
	{
		if (UPlayerStatsComponent* StatsComponent = Character->GetPlayerStatsComponent())
		{
			StatsComponent->SetPersistentStats(SaveData->PlayerStats);
		}
	}

	if (USaveSystem* SaveSystem = GetGameInstance()->GetSubsystem<USaveSystem>())
	{
		SaveSystem->SaveSaveData();
	}

	return true;
}

TArray<FName> URunUpgradeSystem::GetTakenUpgradeIDs() const
{
	return SaveData ? SaveData->TakenUpgradeIDs : TArray<FName>();
}
