// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawner.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "TopDownRPG/Enemy/LockGate.h"
#include "WaveSpawner.generated.h"

USTRUCT(BlueprintType)
struct FSpawnSet
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SpawnCount = 2;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AEnemySpawner*> EnemySpawners;
};

UCLASS()
class TOPDOWNRPG_API AWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AWaveSpawner();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	UBoxComponent* SpawnTrigger;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	TMap<float, FSpawnSet> SpawnSetup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	bool bSpawnSequenceStarted = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	float SpawnSequenceElapsedTime = 0.f;

	UPROPERTY()
	TArray<float> SortedSpawnDelays;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Default")
	int32 NextSpawnDelayIndex = 0;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	ALockGate* StartGate;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Default")
	ALockGate* EndGate;

	UFUNCTION()
	void OnSpawnTriggerBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	void StartSpawnSequence();

	void ExecuteSpawnSet(float Delay);

	void FinishSpawnSequence();
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
