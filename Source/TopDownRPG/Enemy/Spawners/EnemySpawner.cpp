// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AEnemySpawner::Spawn()
{
	UWorld* World = GetWorld();
	if (!World || !EnemyToSpawn)
	{
		return;
	}

	const float CurrentTime = World->GetTimeSeconds();
	const float EffectiveSpawnInterval = FMath::Max(SpawnInterval, 0.1f);
	const float TimeSinceLastSpawn = CurrentTime - LastSpawnTime;

	if (!bHasSpawned || (PendingSpawnCount == 0 && TimeSinceLastSpawn >= EffectiveSpawnInterval))
	{
		SpawnEnemyNow();
		return;
	}

	++PendingSpawnCount;

	if (!World->GetTimerManager().IsTimerActive(SpawnQueueTimerHandle))
	{
		const float Delay = FMath::Max(EffectiveSpawnInterval - TimeSinceLastSpawn, 0.f);
		World->GetTimerManager().SetTimer(
			SpawnQueueTimerHandle,
			this,
			&AEnemySpawner::ProcessSpawnQueue,
			Delay,
			false);
	}
}

void AEnemySpawner::SpawnEnemyNow()
{
	UWorld* World = GetWorld();
	if (!World || !EnemyToSpawn)
	{
		return;
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	const float Angle = FMath::FRandRange(0.f, 2.f * PI);
	const float Distance = FMath::Sqrt(FMath::FRand()) * SpawnRadius;
	const FVector SpawnOffset(FMath::Cos(Angle) * Distance, FMath::Sin(Angle) * Distance, 0.f);
	const FVector SpawnLocation = GetActorLocation() + SpawnOffset;

	World->SpawnActor<AEnemyCharacterBase>(EnemyToSpawn, SpawnLocation, GetActorRotation(), SpawnParameters);

	LastSpawnTime = World->GetTimeSeconds();
	bHasSpawned = true;
}

void AEnemySpawner::ProcessSpawnQueue()
{
	UWorld* World = GetWorld();
	if (!World || PendingSpawnCount <= 0)
	{
		return;
	}

	--PendingSpawnCount;
	SpawnEnemyNow();

	if (PendingSpawnCount > 0)
	{
		const float EffectiveSpawnInterval = FMath::Max(SpawnInterval, 0.1f);
		World->GetTimerManager().SetTimer(
			SpawnQueueTimerHandle,
			this,
			&AEnemySpawner::ProcessSpawnQueue,
			EffectiveSpawnInterval,
			false);
	}
}

