// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TimerManager.h"
#include "TopDownRPG/Enemy/Base/EnemyCharacterBase.h"
#include "EnemySpawner.generated.h"

UCLASS()
class TOPDOWNRPG_API AEnemySpawner : public AActor
{
	GENERATED_BODY()

public:
	AEnemySpawner();

protected:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyCharacterBase> EnemyToSpawn;

	UPROPERTY(EditDefaultsOnly, Category="Spawn")
	float SpawnRadius = 150.f;

	UPROPERTY(EditDefaultsOnly, Category="Spawn", meta=(ClampMin="0.1"))
	float SpawnInterval = 1.f;

	int32 PendingSpawnCount = 0;

	float LastSpawnTime = 0.f;

	bool bHasSpawned = false;

	FTimerHandle SpawnQueueTimerHandle;

	void SpawnEnemyNow();

	void ProcessSpawnQueue();

public:
	UFUNCTION()
	void Spawn();
};
