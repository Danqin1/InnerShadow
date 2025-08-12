// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TopDownRPG/Core/BaseSubsystem.h"
#include "SaveSystem.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API USaveSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	USaveData* SaveData;

public:
	UFUNCTION(BlueprintCallable)
	void LoadSaveData();

	UFUNCTION(BlueprintCallable)
	void SaveSaveData();
	
	void CreateDefaultSaveData();
	void PopulateSystems();

	UFUNCTION(BlueprintCallable)
	USaveData* GetSaveData() const;
};
