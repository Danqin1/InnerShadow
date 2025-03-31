// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "Engine/DataAsset.h"
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

	UPROPERTY(EditAnywhere, Category = "VFX")
	UNiagaraSystem* Blood_FX;
};
