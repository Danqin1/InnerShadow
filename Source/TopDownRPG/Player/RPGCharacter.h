// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/AbilityComponent.h"
#include "Components/CombatComponent.h"
#include "Components/RageComponent.h"
#include "Components/InteractionComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "TopDownRPG/Interfaces/CombatInterface.h"
#include "TopDownRPG/UI/HUD/PlayerHUD.h"
#include "RPGCharacter.generated.h"

UCLASS(Blueprintable)
class ARPGCharacter : public ACharacter, public  IPlayerInterface, public ICombatInterface
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
	bool CanInterruptCurrentAnimation();
	
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
	URageComponent* RageComponent;
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

	UFUNCTION(BlueprintCallable)
	virtual bool Hit(AActor* Hitter, FVector HitPosition, FVector HitVelocity, float Damage, bool canCrushBlock, bool withReaction = true, UAnimMontage* reaction = nullptr) override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool CanDamage() override;
	virtual float GetDarknessPercent() const override;
	virtual void AddPrize(FKillPrize& Prize) override;
	virtual void ResetAttack() override;
	virtual UPlayerStatsComponent* GetPlayerStatsComponent() override { return PlayerStatsComponent; }
	virtual void StopCurrentAnimation() override;

	UFUNCTION(BlueprintCallable)
	virtual bool IsInRage() override;

	UFUNCTION()
	void Die();

	UFUNCTION()
	void Respawn();
};

