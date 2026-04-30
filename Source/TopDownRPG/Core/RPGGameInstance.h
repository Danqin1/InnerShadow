// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "Engine/GameInstance.h"
#include "TopDownRPG/Levels/LevelsDatabase.h"
#include "TopDownRPG/Progression/UpgradesDatabase.h"
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

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Database")
	UUpgradesDatabase* UpgradesDatabase;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings")
	USoundClass* MasterSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings")
	USoundClass* MusicSoundClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Audio Settings")
	TArray<USoundClass*> SFXSoundClasses;

	USoundClass* GetMasterSoundClass() const { return MasterSoundClass; }
	USoundClass* GetMusicSoundClass() const { return MusicSoundClass; }
	const TArray<USoundClass*>& GetSFXSoundClasses() const { return SFXSoundClasses; }
	
	void BeginLoadingScreen();
};
