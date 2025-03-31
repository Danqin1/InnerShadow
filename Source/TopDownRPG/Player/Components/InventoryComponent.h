// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "Components/ActorComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "TopDownRPG/TopDownRPG.h"
#include "InventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UStaticMeshComponent* WeaponMeleeHandle; // for tests
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UStaticMeshComponent* WeaponMeleeHandleLeft; // for tests

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UStaticMesh* DefaultSwordR;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UStaticMesh* DefaultSwordL;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UInputAction* EquipAction;
	
	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UAnimMontage* GetWeaponAnim;

	UPROPERTY(EditDefaultsOnly, Category="Weapon")
	UAnimMontage* HideWeaponAnim;

	bool bEquippedWeapon = false;
	
	UFUNCTION()
	void OnStateChanged(ECharacterState State);
	// Called when the game starts
	virtual void BeginPlay() override;
	
public:

	float GetCurrentWeaponDamage();
	
	UFUNCTION()
	void ToggleMelee();

	void EquipWeapon();
	void HideWeapon();

	inline bool HasEquippedWeapon() { return bEquippedWeapon; }
};
