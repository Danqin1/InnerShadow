// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RPGCharacter.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

	UPROPERTY()
	ARPGCharacter* Character = nullptr;
	UPROPERTY()
	UInventoryComponent* InventoryComponent = nullptr;
	UPROPERTY()
	UCharacterMovementComponent* MovementComponent = nullptr;
public:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsAttacking = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bHasSword = true;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bShouldMove = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	float GroundSpeed = 0;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool bIsFalling = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	FVector Velocity = FVector::Zero();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AccelerationThreshold = 50;

	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
