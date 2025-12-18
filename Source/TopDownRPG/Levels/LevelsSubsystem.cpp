// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelsSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/Core/RPGGameInstance.h"

void ULevelsSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	if (URPGGameInstance* GameInstance = Cast<URPGGameInstance>(GetGameInstance()))
	{
		LevelsDatabase = GameInstance->LevelsDatabase;
	}
}

void ULevelsSubsystem::LoadLevel(FString level)
{
	if (LevelsDatabase->Levels.Contains(level))
	{
		if (URPGGameInstance* GameInstance = Cast<URPGGameInstance>(GetGameInstance()))
		{
			GameInstance->BeginLoadingScreen();
		}
		UGameplayStatics::OpenLevelBySoftObjectPtr(GetWorld(), LevelsDatabase->Levels[level]);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Missing level in database: %s"), *level)
	}
}
