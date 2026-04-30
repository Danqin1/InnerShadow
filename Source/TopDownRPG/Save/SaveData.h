// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TopDownRPG/Progression/RunUpgradeTypes.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/QuestSystem/FQuest.h"
#include "SaveData.generated.h"

USTRUCT(BlueprintType)
struct FPlayerSettingsSaveData
{
	GENERATED_BODY()

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0.0", ClampMax="1.0"))
	float MasterVolume = 1.f;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0.0", ClampMax="1.0"))
	float MusicVolume = 1.f;

	UPROPERTY(SaveGame, BlueprintReadWrite, EditAnywhere, meta=(ClampMin="0.0", ClampMax="1.0"))
	float SFXVolume = 1.f;

	void Clamp()
	{
		MasterVolume = FMath::Clamp(MasterVolume, 0.f, 1.f);
		MusicVolume = FMath::Clamp(MusicVolume, 0.f, 1.f);
		SFXVolume = FMath::Clamp(SFXVolume, 0.f, 1.f);
	}
};

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API USaveData : public USaveGame
{
	GENERATED_BODY()
public:
	UPROPERTY(SaveGame, BlueprintReadWrite)
	FVector PlayerLocation;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FAbilityData> AbilitiesData = {};

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FQuest> ActiveQuests = {};

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<int> CompletedQuests = {};

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FPlayerPersistentStats PlayerStats;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FName> TakenUpgradeIDs = {};

	UPROPERTY(SaveGame, BlueprintReadWrite)
	FPlayerSettingsSaveData PlayerSettings;

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FName> TriggeredRunUpgradeTriggerIDs = {};

	UPROPERTY(SaveGame, BlueprintReadWrite)
	TArray<FName> CompletedWaveSpawnerIDs = {};
};
