// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

#include "ComponentUtils.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "TopDownRPG/Player/RPGCharacter.h"


// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	WeaponMeleeHandle = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMeleeHandle");
	WeaponMeleeHandleLeft = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMeleeBackIdle");

	WeaponMeleeHandle->SetCollisionProfileName("NoCollision");
	WeaponMeleeHandleLeft->SetCollisionProfileName("NoCollision");
}

void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	if(ARPGCharacter* Character  = Cast<ARPGCharacter>(GetOwner()))
	{
		WeaponMeleeHandle->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "weapon_r");
		//WeaponMeleeHandleLeft->AttachToComponent(Character->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "WeaponL");

		Character->OnStateChanged.AddDynamic(this, &UInventoryComponent::OnStateChanged);
	}

	ToggleMelee();
}

void UInventoryComponent::OnStateChanged(ECharacterState State)
{
}

float UInventoryComponent::GetCurrentWeaponDamage()
{
	return 20;// TODO add weapons control
}

void UInventoryComponent::ToggleMelee()
{
	if(ARPGCharacter* Character  = Cast<ARPGCharacter>(GetOwner()))
	{
		if(!bEquippedWeapon)
		{
			if(GetWeaponAnim)
			{
				Character->PlayAnimMontage(GetWeaponAnim);
			}
		}
		else
		{
			if(HideWeaponAnim)
			{
				Character->PlayAnimMontage(HideWeaponAnim);
			}
		}
	}
}

void UInventoryComponent::EquipWeapon()
{
	WeaponMeleeHandle->SetStaticMesh(DefaultSwordR);
	//WeaponMeleeHandleLeft->SetStaticMesh(DefaultSwordL);
	bEquippedWeapon = true;
}

void UInventoryComponent::HideWeapon()
{
	//WeaponMeleeHandleLeft->SetStaticMesh(nullptr);
	WeaponMeleeHandle->SetStaticMesh(nullptr);
	bEquippedWeapon = false;
}

