// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
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

public:
	UFUNCTION()
	void Spawn();
};
