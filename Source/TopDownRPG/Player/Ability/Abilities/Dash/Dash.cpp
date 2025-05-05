// Fill out your copyright notice in the Description page of Project Settings.


#include "Dash.h"

#include "Kismet/KismetMathLibrary.h"
#include "TopDownRPG/Player/RPGCharacter.h"


// Sets default values
ADash::ADash()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ADash::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ADash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ADash::Activate(ACharacter* Caster)
{
	Super::Activate(Caster);

	if (DodgeAnim)
	{
		if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(Caster))
		{
			if (UCharacterMovementComponent* CharacterMovement = RPGPlayer->FindComponentByClass<UCharacterMovementComponent>())
			{
				FVector Direction = RPGPlayer->GetLastMovementInputVector();

				FVector Start = RPGPlayer->GetActorLocation();
				FVector End = Start + Direction * 1000;

				FRotator CurrentRot = Caster->GetActorRotation();
				FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
				FRotator FinalRot = FRotator(CurrentRot.Pitch, TargetRotation.Yaw, TargetRotation.Roll);

				Caster->SetActorRotation(FinalRot);
				CharacterMovement->bOrientRotationToMovement = true;
				RPGPlayer->StopAnimMontage();
				RPGPlayer->PlayAnimMontage(DodgeAnim);

				if (UCombatComponent* CombatComponent = RPGPlayer->FindComponentByClass<UCombatComponent>())
				{
					CombatComponent->ResetAttack();
				}

				FTimerHandle TimerHandle;
				GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, RPGPlayer]
				{
					RPGPlayer->SetState(Nothing);
				},Duration, false, Duration);
			}
		}
	}
}

