#include "UpgradesDatabase.h"

UUpgradesDatabase::UUpgradesDatabase()
{
	if (Upgrades.Num() > 0)
	{
		return;
	}

	auto AddUpgrade = [this](const FName ID, const FString& Name, const FString& Description,
	                         const FPlayerPersistentStats& StatModifiers)
	{
		FRunUpgradeData Upgrade;
		Upgrade.ID = ID;
		Upgrade.Name = Name;
		Upgrade.Description = Description;
		Upgrade.StatModifiers = StatModifiers;
		Upgrades.Add(Upgrade);
	};

	FPlayerPersistentStats Stats;

	Stats.BonusMaxHP = 25.0f;
	AddUpgrade("ThickBlood", "Thick Blood", "+25 max HP.", Stats);

	Stats = FPlayerPersistentStats();
	Stats.DamageMultiplierBonus = 0.15f;
	AddUpgrade("CruelEdge", "Cruel Edge", "+15% damage.", Stats);

	Stats = FPlayerPersistentStats();
	Stats.AttackSpeedMultiplierBonus = 0.15f;
	AddUpgrade("QuickHands", "Quick Hands", "+15% attack speed.", Stats);

	Stats = FPlayerPersistentStats();
	Stats.EssenceGainMultiplierBonus = 0.20f;
	AddUpgrade("OpenVeins", "Open Veins", "+20% Dark Essence gain.", Stats);

	Stats = FPlayerPersistentStats();
	Stats.BonusHPRegen = 1.5f;
	AddUpgrade("ColdRecovery", "Cold Recovery", "+1.5 HP regen.", Stats);

	Stats = FPlayerPersistentStats();
	Stats.IncomingDamageMultiplierBonus = -0.10f;
	AddUpgrade("AshSkin", "Ash Skin", "-10% incoming damage.", Stats);

	Stats = FPlayerPersistentStats();
	Stats.BonusMaxHP = 15.0f;
	Stats.IncomingDamageMultiplierBonus = -0.05f;
	AddUpgrade("IronVow", "Iron Vow", "+15 max HP and -5% incoming damage.", Stats);

	Stats = FPlayerPersistentStats();
	Stats.DamageMultiplierBonus = 0.10f;
	Stats.EssenceGainMultiplierBonus = 0.10f;
	AddUpgrade("DarkHarvest", "Dark Harvest", "+10% damage and +10% Essence gain.", Stats);

	Stats = FPlayerPersistentStats();
	Stats.AttackSpeedMultiplierBonus = 0.10f;
	Stats.BonusHPRegen = 1.0f;
	AddUpgrade("RestlessPulse", "Restless Pulse", "+10% attack speed and +1 HP regen.", Stats);
}

const FRunUpgradeData* UUpgradesDatabase::GetUpgradeByID(FName UpgradeID) const
{
	return Upgrades.FindByPredicate([UpgradeID](const FRunUpgradeData& Upgrade)
	{
		return Upgrade.ID == UpgradeID;
	});
}
