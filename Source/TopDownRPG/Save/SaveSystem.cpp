// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem.h"

#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/Ability/AbilitySystem.h"
#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/Player/Components/PlayerStatsComponent.h"
#include "TopDownRPG/Progression/RunUpgradeSystem.h"
#include "TopDownRPG/QuestSystem/QuestSystem.h"
#include "Sound/SoundClass.h"
#include "Sound/SoundMix.h"
#include "TopDownRPG/Core/RPGGameInstance.h"

void USaveSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void USaveSystem::LoadSaveData()
{
	if (SaveData)
	{
		return;
	}
	if (UGameplayStatics::DoesSaveGameExist("SaveData", 0))
	{
		if (auto* save = Cast<USaveData>(UGameplayStatics::LoadGameFromSlot("SaveData", 0)))
		{
			SaveData = save;
			SaveData->PlayerSettings.Clamp();
			UE_LOG(LogTemp, Log, TEXT("Save data loaded successfully"));
			PopulateSystems();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load save data from slot"));
			CreateDefaultSaveData();
		}
	}
	else
	{
		CreateDefaultSaveData();
		UE_LOG(LogTemp, Log, TEXT("No save data found, created default save data"));
	}
}

void USaveSystem::SaveSaveData()
{
	if (!SaveData)
	{
		LoadSaveData();
	}

	if (!SaveData)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to save data because save data is missing"));
		return;
	}

	if (UGameplayStatics::SaveGameToSlot(SaveData, "SaveData", 0))
	{
		UE_LOG(LogTemp, Log, TEXT("Save data saved successfully"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to save data to slot"));
	}
}

void USaveSystem::CreateDefaultSaveData()
{
	SaveData = NewObject<USaveData>();
	if (SaveData)
	{
		SaveData->PlayerSettings.Clamp();

		if (UAbilitySystem* AbilitySystem = GetGameInstance()->GetSubsystem<UAbilitySystem>())
		{
			SaveData->AbilitiesData = AbilitySystem->GetDefaultData();
			UE_LOG(LogTemp, Log, TEXT("Default save data created with abilities"));
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Ability system not found, default save data created without abilities"));
		}
		
		PopulateSystems();
	}
}

void USaveSystem::PopulateSystems()
{
	if (!SaveData)
	{
		return;
	}

	ApplyPlayerSettings();

	if (UQuestSystem* QuestSystem = Cast<UQuestSystem>(GetGameInstance()->GetSubsystem<UQuestSystem>()))
	{
		QuestSystem->RestoreFromSave(SaveData);
		UE_LOG(LogTemp, Log, TEXT("Quest system populated from save data"));
	}
	if (UAbilitySystem* AbilitySystem = Cast<UAbilitySystem>(GetGameInstance()->GetSubsystem<UAbilitySystem>()))
	{
		AbilitySystem->RestoreFromSave(SaveData);
		UE_LOG(LogTemp, Log, TEXT("Ability system populated from save data"));
	}
	if (URunUpgradeSystem* RunUpgradeSystem = Cast<URunUpgradeSystem>(GetGameInstance()->GetSubsystem<URunUpgradeSystem>()))
	{
		RunUpgradeSystem->RestoreFromSave(SaveData);
		UE_LOG(LogTemp, Log, TEXT("Run upgrade system populated from save data"));
	}
	if (ARPGCharacter* Character = Cast<ARPGCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		if (UPlayerStatsComponent* StatsComponent = Character->GetPlayerStatsComponent())
		{
			StatsComponent->SetPersistentStats(SaveData->PlayerStats);
			UE_LOG(LogTemp, Log, TEXT("Player stats populated from save data"));
		}
	}
}

void USaveSystem::ApplyPlayerSettings()
{
	if (!SaveData)
	{
		return;
	}

	SaveData->PlayerSettings.Clamp();
	EnsureAudioSettingsAssetsLoaded();

	if (!PlayerSettingsSoundMix)
	{
		return;
	}

	UObject* WorldContext = GetGameInstance();
	if (!WorldContext)
	{
		return;
	}

	UGameplayStatics::PushSoundMixModifier(WorldContext, PlayerSettingsSoundMix);

	ApplySoundClassVolume(MasterSoundClass, SaveData->PlayerSettings.MasterVolume, true);
	ApplySoundClassVolume(MusicSoundClass, SaveData->PlayerSettings.MusicVolume, true);

	for (USoundClass* SFXSoundClass : SFXSoundClasses)
	{
		ApplySoundClassVolume(SFXSoundClass, SaveData->PlayerSettings.SFXVolume, true);
	}
}

void USaveSystem::SetPlayerAudioSettings(float MasterVolume, float MusicVolume, float SFXVolume, bool bSaveImmediately)
{
	if (!SaveData)
	{
		LoadSaveData();
	}

	if (!SaveData)
	{
		return;
	}

	SaveData->PlayerSettings.MasterVolume = FMath::Clamp(MasterVolume, 0.f, 1.f);
	SaveData->PlayerSettings.MusicVolume = FMath::Clamp(MusicVolume, 0.f, 1.f);
	SaveData->PlayerSettings.SFXVolume = FMath::Clamp(SFXVolume, 0.f, 1.f);

	ApplyPlayerSettings();

	if (bSaveImmediately)
	{
		SaveSaveData();
	}
}

USaveData* USaveSystem::GetSaveData()
{
	if (!SaveData)
	{
		LoadSaveData();
	}
	return SaveData;
}

void USaveSystem::EnsureAudioSettingsAssetsLoaded()
{
	if (!PlayerSettingsSoundMix)
	{
		PlayerSettingsSoundMix = NewObject<USoundMix>(this, TEXT("PlayerSettingsSoundMix"));
	}

	URPGGameInstance* RPGGameInstance = Cast<URPGGameInstance>(GetGameInstance());
	if (!RPGGameInstance)
	{
		UE_LOG(LogTemp, Warning, TEXT("SaveSystem: RPGGameInstance not found"));
		return;
	}

	MasterSoundClass = RPGGameInstance->GetMasterSoundClass();
	MusicSoundClass = RPGGameInstance->GetMusicSoundClass();
	SFXSoundClasses = RPGGameInstance->GetSFXSoundClasses();
}

void USaveSystem::ApplySoundClassVolume(USoundClass* SoundClass, float Volume, bool bApplyToChildren)
{
	if (!SoundClass || !PlayerSettingsSoundMix)
	{
		return;
	}

	UObject* WorldContext = GetGameInstance();
	if (!WorldContext)
	{
		return;
	}

	UGameplayStatics::SetSoundMixClassOverride(
		WorldContext,
		PlayerSettingsSoundMix,
		SoundClass,
		FMath::Clamp(Volume, 0.f, 1.f),
		1.f,
		0.f,
		bApplyToChildren);
}
