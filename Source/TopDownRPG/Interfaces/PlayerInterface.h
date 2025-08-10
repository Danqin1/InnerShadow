// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/Enemy/FKillPrize.h"
#include "TopDownRPG/Player/Components/PlayerStatsComponent.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UPlayerInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOPDOWNRPG_API IPlayerInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual float GetDarknessPercent() const { return 0; }
	virtual bool IsInRage() {return false;}
	virtual UPlayerStatsComponent* GetPlayerStatsComponent() {return nullptr;}
	
	FStateChanged OnStateChanged;
	
	virtual ECharacterState GetState();
	virtual void SetState(ECharacterState NewState);
	virtual void ClearState(ECharacterState State);

	virtual void AddPrize(FKillPrize& Prize){};
	virtual void ResetAttack(){}
};
