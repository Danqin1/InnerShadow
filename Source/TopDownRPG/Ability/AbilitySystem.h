// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/Save/SaveData.h"
#include "AbilitySystem.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UAbilitySystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USaveData* SaveData;
public:
	UPROPERTY(BlueprintAssignable)
	FAbityEvent OnDataUpdated;
	
	UFUNCTION(BlueprintCallable)
	FAbilityData GetAbilityData(int ID);

	TArray<FAbilityData> GetDefaultData();

	void RestoreFromSave(USaveData* save);
};
