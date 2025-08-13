// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/Save/SaveData.h"
#include "QuestSystem.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UQuestSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	USaveData* SaveData;
public:
	void RestoreFromSave(USaveData* save);
};
