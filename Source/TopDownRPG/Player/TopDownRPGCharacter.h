// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/AbilityComponent.h"
#include "Components/InventoryComponent.h"
#include "Components/PlayerStatsComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "TopDownRPG/Interfaces/IDamageable.h"
#include "TopDownRPG/UI/DamageIndicatorActor.h"
#include "TopDownRPG/UI/HUD/PlayerHUD.h"
#include "TopDownRPGCharacter.generated.h"

UCLASS(Blueprintable)
class ATopDownRPGCharacter : public ACharacter
{
	GENERATED_BODY()
protected:
	/** Top down camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCameraComponent;

	/** Camera boom positioning the camera above the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	UPROPERTY(EditDefaultsOnly, Category="HUD")
	TSubclassOf<class UPlayerHUD> PlayerHUDClass;

	UPROPERTY(EditDefaultsOnly)
	UPlayerStatsComponent* PlayerStatsComponent;
	UPROPERTY(EditDefaultsOnly)
	UAbilityComponent* AbilityComponent;
	UPROPERTY(EditDefaultsOnly)
	UInventoryComponent* InventoryComponent;
	
	UPROPERTY(EditDefaultsOnly, Category = "Sword Trace")
	float SwordTraceRadius = 20;
	UPROPERTY(EditDefaultsOnly, Category = "Sword Trace")
	float SwordTraceDelay = 0.05;

	UPROPERTY(EditDefaultsOnly, Category = "Settings")
	UPlayerSettings* Settings = nullptr;

	UPROPERTY(EditDefaultsOnly, Category = "Indication")
	TSubclassOf<ADamageIndicatorActor> DamageIndicator;

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	UAIPerceptionStimuliSourceComponent* StimulusSourceComponent;

	bool bIsTracingSword = false;
	float lastSwordTrace = 0;

	TArray<IIDamageable*> DamagedActors;

	float CurrentDamage = 0;
public:
	ATopDownRPGCharacter();

	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Called every frame.
	virtual void Tick(float DeltaSeconds) override;

	/** Returns TopDownCameraComponent subobject **/
	FORCEINLINE class UCameraComponent* GetTopDownCameraComponent() const { return TopDownCameraComponent; }
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	UPROPERTY()
	UPlayerHUD* PlayerHUD;

	bool bIsAttacking = false;

	void SetAutoAttack(bool enabled);
	void StartSwordTrace();
	void EndSwordTrace();
	void ModifyDamage(float NewDamage);
	void ClearDamageModifier();
	void TryDamageByAbility(const FVector Position, float Damage, const float Range = 10);
};

