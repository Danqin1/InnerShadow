// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGActorComponentBase.h"
#include "Components/ActorComponent.h"
#include "TopDownRPG/Progression/RunUpgradeTypes.h"
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

	UPROPERTY(EditAnywhere, Category="Stats")
	float BaseMaxHP = 100;

	UPROPERTY(EditAnywhere, Category="Stats")
	float BaseMaxEssence = 100;

	float HP = 100;
	float MaxHP = 100;
	float XP = 0;

	float Essence = 0;
	float MaxEssence = 100;
	float DurFromLastEssenceInput = 0;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Stats", meta=(AllowPrivateAccess="true"))
	FPlayerPersistentStats PersistentStats;

	void UpdateHUD();
	void RecalculateDerivedStats();
	void LoadPersistentStatsFromSave();
	
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

	UFUNCTION(BlueprintCallable)
	void SetPersistentStats(const FPlayerPersistentStats& InStats);

	UFUNCTION(BlueprintCallable)
	FPlayerPersistentStats GetPersistentStats() const { return PersistentStats; }

	void AddEssence(float Value);
	void RemoveEssence(float Value);
	float GetHP() const { return HP; }
	float GetMaxHP() const { return MaxHP; }
	float GetEssence() const { return Essence; }
	float GetMaxEssence() const { return MaxEssence; }
	float GetEssencePercent() const;
	EDarkEssenceState GetEssenceState() const;
	float GetOutgoingDamageMultiplier() const;
	float GetIncomingDamageMultiplier() const;
	float GetAttackSpeedMultiplier() const;
	float GetHealthRegenPerSecond() const;
	float GetEssenceGainMultiplier() const;
};
