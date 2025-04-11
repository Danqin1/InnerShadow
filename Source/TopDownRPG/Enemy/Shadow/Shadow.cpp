// Fill out your copyright notice in the Description page of Project Settings.


#include "Shadow.h"

#include "ShadowCombat.h"


// Sets default values
AShadow::AShadow()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Combat = CreateDefaultSubobject<UShadowCombat>("Combat");
}

// Called when the game starts or when spawned
void AShadow::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AShadow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AShadow::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

