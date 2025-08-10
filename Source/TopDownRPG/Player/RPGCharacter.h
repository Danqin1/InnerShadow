// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/AbilityComponent.h"
#include "Components/CombatComponent.h"
#include "Components/DarknessComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "TopDownRPG/UI/HUD/PlayerHUD.h"
#include "RPGCharacter.generated.h"

UCLASS(Blueprintable)
class ARPGCharacter : public ACharacter, public  IPlayerInterface, public IDamageableInterface
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	UPlayerSettings* Settings = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimulusSourceComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;
	
	FVector StartLocation;
	FRotator StartRotation;

protected:
	bool bIsFlying = false;
	ECharacterState PlayerState = Dead;
	ECharacterState PreviousState = Dead;
	
	// To add mapping context
	virtual void BeginPlay();
	virtual void Tick(float DeltaSeconds) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	
public:
	ARPGCharacter();
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UPlayerStatsComponent* PlayerStatsComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAbilityComponent* AbilityComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInventoryComponent* InventoryComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UCombatComponent* CombatComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UDarknessComponent* DarknessComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character")
	USkeletalMesh* CharacterMesh;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Character")
	TSubclassOf<UAnimInstance> CharacterAnimBP;
	
	UPROPERTY(BlueprintReadWrite)
	UPlayerHUD* PlayerHUD;
	
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	virtual void AddMovementInput(FVector WorldDirection, float ScaleValue, bool bForce) override;

	UFUNCTION(BlueprintCallable)
	virtual ECharacterState GetState() override;

	UFUNCTION(BlueprintCallable)
	virtual void SetState(ECharacterState NewState) override;

	UFUNCTION(BlueprintCallable)
	virtual void ClearState(ECharacterState State) override;

	virtual void Damage(float Damage) override;
	virtual bool CanDamage() override;
	virtual float GetDarknessPercent() const override;
	virtual void AddPrize(FKillPrize& Prize) override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsDark() override;

	UFUNCTION()
	void Die();

	UFUNCTION()
	void Respawn();
};

