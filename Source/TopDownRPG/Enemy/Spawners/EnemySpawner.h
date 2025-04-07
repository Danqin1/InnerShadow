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
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	UPROPERTY(Transient)
	TArray<AActor*> SpawnedEnemies;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<AEnemyCharacterBase> EnemyToSpawn;
	UPROPERTY(EditAnywhere)
	int MaxEnemies = 1;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
