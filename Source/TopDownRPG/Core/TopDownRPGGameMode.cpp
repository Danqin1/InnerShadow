// Copyright Epic Games, Inc. All Rights Reserved.

#include "TopDownRPGGameMode.h"
#include "TopDownRPG//Player/RPGPlayerController.h"
#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/Save/SaveSystem.h"
#include "UObject/ConstructorHelpers.h"

ATopDownRPGGameMode::ATopDownRPGGameMode()
{
	// use our custom PlayerController class
	PlayerControllerClass = ARPGPlayerController::StaticClass();

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_TopDownCharacter"));
	if (PlayerPawnBPClass.Class != nullptr)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	// set default controller to our Blueprinted controller
	static ConstructorHelpers::FClassFinder<APlayerController> PlayerControllerBPClass(TEXT("/Game/Blueprints/BP_TopDownPlayerController"));
	if(PlayerControllerBPClass.Class != NULL)
	{
		PlayerControllerClass = PlayerControllerBPClass.Class;
	}

	
}

void ATopDownRPGGameMode::BeginPlay()
{
	if (USaveSystem* SaveSystem = GetGameInstance()->GetSubsystem<USaveSystem>())
	{
		SaveSystem->LoadSaveData();
	}
	Super::BeginPlay();
}
