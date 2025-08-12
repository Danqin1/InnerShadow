// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TopDownRPG/Save/SaveData.h"
#include "BaseSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UBaseSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
public:
	virtual void RestoreFromSave(USaveData* data){};
};
