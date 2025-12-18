// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Engine/GameInstance.h"
#include "TopDownRPG/Levels/LevelsDatabase.h"
#include "RPGGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API URPGGameInstance : public UGameInstance
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCommonUserWidget> LoadingScreen;
public:
	UPROPERTY(EditAnywhere)
	ULevelsDatabase* LevelsDatabase;
	
	void BeginLoadingScreen();
};
