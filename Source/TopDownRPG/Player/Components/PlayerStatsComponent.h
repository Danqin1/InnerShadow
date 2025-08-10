// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGActorComponentBase.h"
#include "Components/ActorComponent.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/Database/FEnemyData.h"
#include "TopDownRPG/UI/HUD/PlayerHUD.h"
#include "PlayerStatsComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API UPlayerStatsComponent : public URPGActorComponentBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(Transient)
	UPlayerHUD* PlayerHUD = nullptr;

	float HP = 100;
	float MaxHP = 100;
	float XP = 0;

	float Essence = 100;
	float MaxEssence = 100;

	void UpdateHUD();
	
public:
	FDynamicEvent OnDied;
	
	// Sets default values for this component's properties
	UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void SetupComponent(UPlayerSettings* Settings) override;
	virtual void Dispose() override;

	UFUNCTION(BlueprintCallable)
	void AddHP(float Value);

	UFUNCTION(BlueprintCallable)
	void RemoveHP(float Value);
	
	void AddMaxHP(float Value);
	void SetMaxHP(float Value);
	void AddXP(int xp);

	void AddEssence(float Value);
	void RemoveEssence(float Value);
	float GetEssence() {return Essence;}
};
