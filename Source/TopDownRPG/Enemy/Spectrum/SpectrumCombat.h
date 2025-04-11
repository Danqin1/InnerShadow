// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/Enemy/Base/EnemyCombatBase.h"
#include "TopDownRPG/Enemy/Spawners/EnemySpawner.h"
#include "SpectrumCombat.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API USpectrumCombat : public UEnemyCombatBase
{
	GENERATED_BODY()
	
	USpectrumCombat();

protected:
	UPROPERTY(EditAnywhere)
	TArray<AEnemySpawner*> EnemySpawners;

	UPROPERTY(EditAnywhere, Category="Combat")
	TMap<int, bool> EnemiesSpawnThresholds;

	UPROPERTY(EditAnywhere, Category="Combat")
	float AttackDelay = 3;
	
	float lastAttackTime = 0;

	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnDamaged(float CurrentHealth) override;
	virtual void Attack() override;
	virtual bool CanAttack() override;
};
