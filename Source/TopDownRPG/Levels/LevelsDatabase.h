// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "LevelsDatabase.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API ULevelsDatabase : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TMap<FString, TSoftObjectPtr<UWorld>> Levels;
};
