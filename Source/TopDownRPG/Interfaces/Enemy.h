// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/Enemy/FKillPrize.h"
#include "UObject/Interface.h"
#include "Enemy.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UEnemy : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */

class TOPDOWNRPG_API IEnemy
{
	GENERATED_BODY()
	
	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
protected:
public:
	FDynamicEvent OnDie;
	
	UFUNCTION()
	virtual void OnHit(AActor* Hitter, FVector HitPosition, FVector HitVelocity) PURE_VIRTUAL()
	UFUNCTION()
	virtual void OnSkillReaction(UAnimMontage* ReactionMontage) PURE_VIRTUAL()
	UFUNCTION()
	virtual FKillPrize GetPrize() {return FKillPrize();}
	virtual void ChangeMovementMode(EMovementType NewMovement) {}
};
