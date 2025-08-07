// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilityEffect.h"
#include "GameFramework/Actor.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/UI/HUD/W_ActionSlot.h"
#include "Ability.generated.h"

UCLASS(Abstract)
class TOPDOWNRPG_API AAbility : public AActor
{
	GENERATED_BODY()

protected:
	float RechargeTime = 0;
	
	UPROPERTY()
	UW_ActionSlot* UISlot;
public:
	// Sets default values for this actor's properties
	AAbility();

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	bool bRequiresDarkness = true;

	UPROPERTY(EditDefaultsOnly, Category="Ability")
	float Cooldown = 5;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	FString Name = "";
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	FString Description = "";
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	UTexture2D* Icon = nullptr;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TEnumAsByte<ECastType> CastType = None;
	
	UPROPERTY(EditDefaultsOnly, Category="Ability")
	TArray<TSubclassOf<class AAbilityEffect>> Effects;

	UPROPERTY()
	ACharacter* CasterCharacter;

	UPROPERTY()
	ACharacter* TargetCharacter;

	UPROPERTY()
	FVector CastLocation;
	
	virtual bool CanUseAbility();
	virtual void Activate(ACharacter* Caster);
	virtual void SetUISlot(UW_ActionSlot* Slot);
	virtual void Tick(float DeltaSeconds) override;
};
