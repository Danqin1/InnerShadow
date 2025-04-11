// Fill out your copyright notice in the Description page of Project Settings.


#include "Spectrum.h"

#include "SpectrumCombat.h"


// Sets default values
ASpectrum::ASpectrum()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Combat = CreateDefaultSubobject<USpectrumCombat>("Combat");
}

// Called when the game starts or when spawned
void ASpectrum::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpectrum::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void ASpectrum::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

