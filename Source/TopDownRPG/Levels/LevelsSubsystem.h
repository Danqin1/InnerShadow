// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LevelsDatabase.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "LevelsSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API ULevelsSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	ULevelsDatabase* LevelsDatabase;
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	void LoadLevel(FString level);
};
