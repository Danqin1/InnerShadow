// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem.h"

#include "TopDownRPG/DevDebug.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/Save/SaveSystem.h"

FAbilityData UAbilitySystem::GetAbilityData(int id)
{
	if (SaveData)
	{
		auto data = SaveData->AbilitiesData.FindByPredicate([id](const FAbilityData& data) 
		{
			return data.ID == id;
		});

		if (data)
		{
			DevDebug::OnScreenLog("Ability found for ID: " + FString::FromInt(id), FColor::Green);
			return *data;
		}
		else
		{
			DevDebug::OnScreenLog("Ability data not found for ID: " + FString::FromInt(id), FColor::Red);
			FAbilityData newData;
			newData.ID = id;
			newData.bIsUnlocked = false;
			newData.Level = 0;
			SaveData->AbilitiesData.Add(newData);

			return newData;
		}
	}

	DevDebug::OnScreenLog("UAbilitySystem::GetAbilityData Save data not exists yet: " + FString::FromInt(id), FColor::Red);
	FAbilityData newData;
	newData.ID = id;
	newData.bIsUnlocked = false;
	newData.Level = 0;

	return newData;
}

TArray<FAbilityData> UAbilitySystem::GetDefaultData()
{
	TArray<FAbilityData> data;
	for (int i = 0; i < 10; ++i)
	{
		FAbilityData newData;
		newData.ID = i;
		newData.bIsUnlocked = false;
		newData.Level = 0;
		data.Add(newData);
	}

	return data;
}

void UAbilitySystem::RestoreFromSave(USaveData* save)
{
	SaveData = save;
}
