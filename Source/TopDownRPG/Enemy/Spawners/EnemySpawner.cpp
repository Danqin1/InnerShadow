// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/DevDebug.h"


// Sets default values
AEnemySpawner::AEnemySpawner()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 5;
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UEnemy::StaticClass(), SpawnedEnemies);

	DevDebug::OnScreenLog("Enemies around: "+ FString::SanitizeFloat(SpawnedEnemies.Num()));
}

// Called every 5s
void AEnemySpawner::Tick(float DeltaTime)
{
	FVector Location = GetActorLocation();

	UGameplayStatics::GetAllActorsWithInterface(GetWorld(), UEnemy::StaticClass(), SpawnedEnemies);
	if(SpawnedEnemies.Num() > MaxEnemies)
	{
		return;
	}
	
	auto* Enemy = GetWorld()->SpawnActor<AEnemyCharacterBase>(EnemyToSpawn, Location, FRotator::ZeroRotator);
}

