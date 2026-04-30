// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SaveSystem.generated.h"

class USoundClass;
class USoundMix;

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

	UPROPERTY(Transient)
	USoundMix* PlayerSettingsSoundMix = nullptr;

	UPROPERTY(Transient)
	USoundClass* MasterSoundClass = nullptr;

	UPROPERTY(Transient)
	USoundClass* MusicSoundClass = nullptr;

	UPROPERTY(Transient)
	TArray<USoundClass*> SFXSoundClasses;

private:
	void EnsureAudioSettingsAssetsLoaded();
	void ApplySoundClassVolume(USoundClass* SoundClass, float Volume, bool bApplyToChildren);

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	UFUNCTION(BlueprintCallable)
	void LoadSaveData();

	UFUNCTION(BlueprintCallable)
	void SaveSaveData();
	
	void CreateDefaultSaveData();
	void PopulateSystems();

	UFUNCTION(BlueprintCallable)
	void ApplyPlayerSettings();

	UFUNCTION(BlueprintCallable)
	void SetPlayerAudioSettings(float MasterVolume, float MusicVolume, float SFXVolume, bool bSaveImmediately = true);

	UFUNCTION(BlueprintCallable)
	bool HasRunUpgradeTriggerBeenTriggered(FName TriggerID);

	UFUNCTION(BlueprintCallable)
	void SetRunUpgradeTriggerTriggered(FName TriggerID, bool bTriggered, bool bSaveImmediately = true);

	UFUNCTION(BlueprintCallable)
	bool IsWaveSpawnerCompleted(FName SpawnerID);

	UFUNCTION(BlueprintCallable)
	void SetWaveSpawnerCompleted(FName SpawnerID, bool bCompleted, bool bSaveImmediately = true);

	UFUNCTION(BlueprintCallable)
	USaveData* GetSaveData();
};
