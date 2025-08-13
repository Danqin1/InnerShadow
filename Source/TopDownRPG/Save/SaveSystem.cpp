// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem.h"

#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/Ability/AbilitySystem.h"
#include "TopDownRPG/QuestSystem/QuestSystem.h"

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
			UE_LOG(LogTemp, Log, TEXT("Save data loaded successfully"));
			PopulateSystems();
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Failed to load save data from slot"));
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
}

USaveData* USaveSystem::GetSaveData()
{
	if (!SaveData)
	{
		CreateDefaultSaveData();
	}
	return SaveData;
}
