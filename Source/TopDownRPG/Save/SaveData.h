// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "TopDownRPG/TopDownRPG.h"
#include "SaveData.generated.h"
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
};