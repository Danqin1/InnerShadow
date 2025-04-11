// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AEnemySpawner::Spawn()
{
	auto* Enemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(EnemyToSpawn, GetActorLocation(), GetActorRotation());
}

