// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TopDownRPG/DevDebug.h"

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	if(ARPGCharacter* Player  = Cast<ARPGCharacter>(TryGetPawnOwner()))
	{
		Character = Player;
		if(UInventoryComponent* Inventory = Player->FindComponentByClass<UInventoryComponent>())
		{
			InventoryComponent = Inventory;
		}
		MovementComponent = Player->GetCharacterMovement();
	}
	Super::NativeInitializeAnimation();
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if(Character && MovementComponent)
	{
		Velocity = Character->GetVelocity();
		GroundSpeed = UKismetMathLibrary::VSizeXY(Velocity);
		bShouldMove = GroundSpeed > 3 && MovementComponent->GetCurrentAcceleration() != FVector::Zero();
		bIsFalling = MovementComponent->IsFalling();
		if(InventoryComponent)
		{
			bHasSword = InventoryComponent->HasEquippedWeapon();
		}
	}
}
