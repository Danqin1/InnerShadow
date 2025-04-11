// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectrumCombat.h"

#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/DevDebug.h"

USpectrumCombat::USpectrumCombat()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USpectrumCombat::BeginPlay()
{
	Super::BeginPlay();
}

void USpectrumCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USpectrumCombat::OnDamaged(float CurrentHealth)
{
	Super::OnDamaged(CurrentHealth);
	for (TTuple<int, bool> EnemiesSpawnThreshold : EnemiesSpawnThresholds)
	{
		if (EnemiesSpawnThreshold.Key < CurrentHealth && !EnemiesSpawnThreshold.Value)
		{
			EnemiesSpawnThreshold.Value = true;
			for (AEnemySpawner* EnemySpawner : EnemySpawners)
			{
				EnemySpawner->Spawn();
			}

			return;
		}
	}
}

void USpectrumCombat::Attack()
{
	Super::Attack();
	lastAttackTime = UGameplayStatics::GetTimeSeconds(GetWorld());
	DevDebug::OnScreenLog("Spectrum attack");
}

bool USpectrumCombat::CanAttack()
{
	return UGameplayStatics::GetTimeSeconds(GetWorld()) - lastAttackTime > AttackDelay;
}

