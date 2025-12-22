// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/Enemy/FKillPrize.h"
#include "TopDownRPG/Player/Components/PlayerStatsComponent.h"
#include "UObject/Interface.h"
#include "PlayerInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
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
	UFUNCTION(BlueprintCallable)
	virtual float GetDarknessPercent() const { return 0; }
	
	UFUNCTION(BlueprintCallable)
	virtual bool IsInRage() {return false;}
	
	UFUNCTION(BlueprintCallable)
	virtual UPlayerStatsComponent* GetPlayerStatsComponent() {return nullptr;}
	
	UFUNCTION(BlueprintCallable)
	virtual void StopCurrentAnimation() {};

	FStateChanged OnStateChanged;
	
	UFUNCTION(BlueprintCallable)
	virtual ECharacterState GetState();
	
	UFUNCTION(BlueprintCallable)
	virtual void SetState(ECharacterState NewState);
	
	UFUNCTION(BlueprintCallable)
	virtual void ClearState(ECharacterState State);

	UFUNCTION(BlueprintCallable)
	virtual void AddPrize(FKillPrize& Prize){};
	
	UFUNCTION(BlueprintCallable)
	virtual void ResetAttack(){}
};
