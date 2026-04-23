#pragma once

#include "CoreMinimal.h"
#include "RunUpgradeTypes.generated.h"

USTRUCT(BlueprintType)
struct TOPDOWNRPG_API FPlayerPersistentStats
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BonusMaxHP = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BonusHPRegen = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DamageMultiplierBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float IncomingDamageMultiplierBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackSpeedMultiplierBonus = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EssenceGainMultiplierBonus = 0.0f;
};

USTRUCT(BlueprintType)
struct TOPDOWNRPG_API FRunUpgradeData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ID = NAME_None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FPlayerPersistentStats StatModifiers;
};
