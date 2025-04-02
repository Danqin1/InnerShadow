// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability_ShadowBlast.h"

#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "TopDownRPG/DevDebug.h"


// Sets default values
AAbility_ShadowBlast::AAbility_ShadowBlast()
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void AAbility_ShadowBlast::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAbility_ShadowBlast::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DevDebug::OnScreenLog("Shadown blast recharge:" +FString::SanitizeFloat(RechargeTime), FColor::Black, .1);
}

void AAbility_ShadowBlast::Activate(ACharacter* Caster)
{
	Super::Activate(Caster);
}

