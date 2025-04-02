// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/Player/Ability/AbilityEffect.h"
#include "AbilityEffect_ShadowBlast.generated.h"

UCLASS()
class TOPDOWNRPG_API AAbilityEffect_ShadowBlast : public AAbilityEffect
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAbilityEffect_ShadowBlast();
protected:
	UPROPERTY(EditDefaultsOnly)
	float ImpactDelay = 4;

	UPROPERTY(EditDefaultsOnly)
	float TraceRadius = 100;

	UPROPERTY(EditDefaultsOnly)
	FVector VFXOffset = FVector(0, 0, 0);

	UPROPERTY(EditDefaultsOnly)
	float TraceLength = 700;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* BlastAnimation = nullptr;
	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* BlastReactionAnimation = nullptr;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
public:

	virtual void Activate(ACharacter* Caster) override;
};
