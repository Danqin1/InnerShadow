// Fill out your copyright notice in the Description page of Project Settings.


#include "LockGate.h"


// Sets default values
ALockGate::ALockGate()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SetRootComponent(CreateDefaultSubobject<USceneComponent>("RootSceneComponent"));
}

// Called when the game starts or when spawned
void ALockGate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALockGate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

