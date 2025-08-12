// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/Core/BaseSubsystem.h"
#include "QuestSystem.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UQuestSystem : public UBaseSubsystem
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	USaveData* saveData;
public:
	virtual void RestoreFromSave(USaveData* data) override;
};
