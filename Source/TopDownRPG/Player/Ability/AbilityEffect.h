// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NiagaraSystem.h"
#include "GameFramework/Actor.h"
#include "AbilityEffect.generated.h"


UCLASS(Abstract)
class TOPDOWNRPG_API AAbilityEffect : public AActor
{
	GENERATED_BODY()

public:
	AAbilityEffect();

	virtual void Activate(ACharacter* Caster);
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Damage")
	float Damage = 1;
	
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float EffectDuration = 1;

	UPROPERTY(BlueprintReadOnly)
	ACharacter* Player = nullptr;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_Activate(ACharacter* Caster);
	
	UFUNCTION(BlueprintCallable)
	void OnEffectFinished();
};
