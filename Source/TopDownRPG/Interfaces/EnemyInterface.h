// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/Enemy/FKillPrize.h"
#include "EnemyInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI,meta=(CannotImplementInterfaceInBlueprint))
class UEnemyInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOPDOWNRPG_API IEnemyInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	FDynamicEvent OnDie;

	FStateChanged OnStateChanged;
	
	virtual ECharacterState GetState();
	virtual void SetState(ECharacterState NewState, bool force = false);
	virtual void ClearState(ECharacterState State);
	
	UFUNCTION(BlueprintCallable)
	virtual FKillPrize GetPrize() {return FKillPrize();}
	virtual void ChangeMovementMode(EMovementType NewMovement) {}

	UFUNCTION(BlueprintCallable)
	virtual void Freeze(bool isFrozen) PURE_VIRTUAL();
	
	UFUNCTION(BlueprintCallable)
	virtual void ChargeAttack(bool start) PURE_VIRTUAL();
};
