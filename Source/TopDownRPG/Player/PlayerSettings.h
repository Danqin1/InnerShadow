// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CameraShake.h"
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
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float MeleeAttackRange = 300;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float MeleeBaseDamage = 10;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AutoTargetRange = 500;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float AutoPickUpWeaponRange = 800;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* AttackAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Input")
	UInputAction* DodgeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Input")
	const UInputAction* RageAction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Indication")
	TSubclassOf<ADamageIndicatorActor> DamageIndicator;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Indication")
	UNiagaraSystem* BloodVFX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sword Trace")
	float SwordTraceRadius = 20;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Sword Trace")
	float SwordTraceDelay = 0.05;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TArray<UAnimMontage*> NormalAttackComboAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TArray<UAnimMontage*> DragonAttackComboAnimations;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	UAnimMontage* AfterDodgeAttack;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	UAnimMontage* DodgeAnim;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	UAnimMontage* HitReaction;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	TSubclassOf<UCameraShake> SwordHitCameraShake;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float SoftLockDetectionRadius = 300;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float SoftLockDetectionDistance = 600;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	UNiagaraSystem* SoftLockTargetDashVFX = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float PushEnemiesStrength = 10;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float DashDarknessCost = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	float SoftLockRotationSpeed =5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat")
	bool UseSoftLock = false;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Combat")
	bool CanDamageDuringSkill = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "VFX")
	UNiagaraSystem* Blood_FX;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Regen")
	float DuringRageEssenceRemoveRate = 40;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float EmpoweredEssenceThreshold = 0.4f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float UnstableEssenceThreshold = 0.8f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float EmpoweredDamageMultiplier = 1.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float UnstableDamageMultiplier = 1.35f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float FrenzyDamageMultiplier = 2.0f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float UnstableIncomingDamageMultiplier = 1.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float FrenzyIncomingDamageMultiplier = 1.35f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float EmpoweredAttackSpeedMultiplier = 1.1f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float UnstableAttackSpeedMultiplier = 1.2f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float FrenzyAttackSpeedMultiplier = 1.6f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float FrenzyMovementControlMultiplier = 0.4f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Dark Essence")
	float FrenzyRecoveryDuration = 0.75f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Regen")
	float RageEssenceClearRate = 10;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Regen")
	float HPRegen = .01;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Interaction")
	float InteractionMaxDistance = 1000;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Ability1Action;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Ability2Action;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Ability3Action;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* Ability4Action;
	UPROPERTY(EditDefaultsOnly, Category="Input")
	UInputAction* BlockAction;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<AAbility> Ability1;
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<AAbility> Ability2;
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<AAbility> Ability3;
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TSubclassOf<AAbility> Ability4;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	USoundBase* OnHitSound;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Audio")
	float HitAudioMinDelay = 0.1f;
};
