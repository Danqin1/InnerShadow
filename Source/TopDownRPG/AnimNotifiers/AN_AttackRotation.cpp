// Fill out your copyright notice in the Description page of Project Settings.


#include "AN_AttackRotation.h"

#include "TopDownRPG/Player/RPGCharacter.h"

void UAN_AttackRotation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation,
                                     float TotalDuration)
{
	if(auto* Character = Cast<ARPGCharacter>(MeshComp->GetOwner()))
	{
		if(!Character->CombatComponent->GetSoftLockTarget())
		{
			Character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = true;
		}
	}
}

void UAN_AttackRotation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if(auto* Character = Cast<ARPGCharacter>(MeshComp->GetOwner()))
	{
		Character->GetCharacterMovement()->bAllowPhysicsRotationDuringAnimRootMotion = false;
	}
}
