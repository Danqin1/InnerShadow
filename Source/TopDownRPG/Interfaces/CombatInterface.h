// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CombatInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(BlueprintType, MinimalAPI, meta=(CannotImplementInterfaceInBlueprint))
class UCombatInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TOPDOWNRPG_API ICombatInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintCallable)
	virtual bool Hit(AActor* Hitter, FVector HitPosition, FVector HitVelocity, float damage, bool canCrushBlock,
		bool withReaction = true, UAnimMontage* reaction = nullptr) {return true;};

	UFUNCTION(BlueprintCallable)
	virtual float PerformAttack() {return 0;}
	
	UFUNCTION(BlueprintCallable)
	virtual bool CanAttack() {return true;}
	
	UFUNCTION(BlueprintCallable)
	virtual void StartTraceAttack(){}
	
	UFUNCTION(BlueprintCallable)
	virtual void EndTraceAttack(){}
	
	UFUNCTION(BlueprintCallable)
	virtual bool CanDamage() {return true;}
	
	UFUNCTION(BlueprintCallable)
	virtual bool WasBlocked(){return false;}
	
	UFUNCTION(BlueprintCallable)
	virtual void OnSkillReaction(UAnimMontage* ReactionMontage) {}
};
