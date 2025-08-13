// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "TopDownRPGGameMode.generated.h"

UCLASS(minimalapi)
class ATopDownRPGGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATopDownRPGGameMode();

	virtual void BeginPlay() override;
};



