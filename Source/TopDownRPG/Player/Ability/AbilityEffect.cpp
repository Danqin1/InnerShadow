// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityEffect.h"

#include "TopDownRPG/DevDebug.h"
#include "TopDownRPG/Player/RPGCharacter.h"


// Sets default values
AAbilityEffect::AAbilityEffect()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AAbilityEffect::Activate(ACharacter* Caster)
{
	Player = Caster;
	BP_Activate(Player);
}

// Called when the game starts or when spawned
void AAbilityEffect::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityEffect::OnEffectFinished()
{
	if (ARPGCharacter* CharacterState = Cast<ARPGCharacter>(Player))
	{
		CharacterState->ClearState(Skill);
	}
	else
	{
		DevDebug::OnScreenLog("Cant cast to character");
	}

	Destroy();
}

