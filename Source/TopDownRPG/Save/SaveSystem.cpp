// Fill out your copyright notice in the Description page of Project Settings.


#include "SaveSystem.h"

#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/Core/BaseSubsystem.h"

void USaveSystem::LoadSaveData()
{
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
		PopulateSystems();
	}
}

void USaveSystem::PopulateSystems()
{
	TFunctionRef<void(UGameInstanceSubsystem*)> restoreFunc = [this](UGameInstanceSubsystem* Subsystem)
	{
		if (UBaseSubsystem* system = Cast<UBaseSubsystem>(Subsystem))
		{
			system->RestoreFromSave(SaveData);
			UE_LOG(LogTemp, Log, TEXT("Restored subsystem: %s"), *Subsystem->GetName());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Subsystem is null"));
		}
	};
	GetGameInstance()->ForEachSubsystem(restoreFunc);
}

USaveData* USaveSystem::GetSaveData() const
{
	return SaveData;
}
