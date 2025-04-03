// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HitCameraShake.h"
#include "InputAction.h"
#include "NiagaraSystem.h"
#include "Ability/Ability.h"
#include "Engine/DataAsset.h"
#include "TopDownRPG/UI/DamageIndicatorActor.h"
#include "PlayerSettings.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class TOPDOWNRPG_API UPlayerSettings : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeAttackRange = 300;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float MeleeBaseDamage = 10;
	
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AutoTargetRange = 500;
	UPROPERTY(EditAnywhere, Category = "Combat")
	float AutoPickUpWeaponRange = 800;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* AttackAction;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* DodgeAction;
	
	UPROPERTY(EditDefaultsOnly, Category = "Indication")
	TSubclassOf<ADamageIndicatorActor> DamageIndicator;

	UPROPERTY(EditDefaultsOnly, Category = "Indication")
	UNiagaraSystem* BloodVFX;

	UPROPERTY(EditDefaultsOnly, Category = "Sword Trace")
	float SwordTraceRadius = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Sword Trace")
	float SwordTraceDelay = 0.05;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TArray<UAnimMontage*> NormalAttackComboAnimations;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TArray<UAnimMontage*> DragonAttackComboAnimations;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* AfterDodgeAttack;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* DodgeAnim;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* DieAnim;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	TSubclassOf<UHitCameraShake> SwordHitCameraShake;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float PushEnemiesStrength = 10;

	UPROPERTY(EditAnywhere, Category = "Combat")
	float DashDarknessCost = 5;

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* Blood_FX;

	UPROPERTY(EditDefaultsOnly, Category="Regen")
	float DarknessRegen = 40;

	UPROPERTY(EditDefaultsOnly, Category="Regen")
	float DarknessDefaultRegen = 10;
	
	UPROPERTY(EditDefaultsOnly, Category="Regen")
	float HPRegen = .01;

	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float InteractionMaxDistance = 1000;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Ability1Action;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Ability2Action;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Ability3Action;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Ability4Action;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<AAbility> Ability1;
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<AAbility> Ability2;
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<AAbility> Ability3;
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<AAbility> Ability4;
};
