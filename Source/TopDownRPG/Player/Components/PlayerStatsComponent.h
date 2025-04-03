// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGActorComponentBase.h"
#include "Components/ActorComponent.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/UI/HUD/PlayerHUD.h"
#include "PlayerStatsComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API UPlayerStatsComponent : public URPGActorComponentBase
{
	GENERATED_BODY()
public:
	FDynamicEvent OnDied;
	
	// Sets default values for this component's properties
	UPlayerStatsComponent(const FObjectInitializer& ObjectInitializer);
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	virtual void SetupComponent(UPlayerSettings* Settings) override;
	virtual void Dispose() override;

	void AddHP(float Value);
	void RemoveHP(float Value);
	void AddMaxHP(float Value);
	void SetMaxHP(float Value);
	
	void AddDarkness(float Value);
	void RemoveDarkness(float Value);
protected:
	UPROPERTY(Transient)
	UPlayerHUD* PlayerHUD = nullptr;

	float HP = 100;
	float MaxHP = 100;
	float Darkness = 0;
	float MaxDarkness = 100;

	void UpdateHUD();
};
