// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "InventoryComponent.h"
#include "NiagaraSystem.h"
#include "RPGActorComponentBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "TopDownRPG/Interfaces/CombatInterface.h"
#include "TopDownRPG/Interfaces/PlayerInterface.h"
#include "CombatComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API UCombatComponent : public URPGActorComponentBase
{
	GENERATED_BODY()

	bool bIsTracingSword = false;
	/*bool bTimeSlowed = false;
	float timeSLowDuration = 0;
	bool bCanSlowTime = false;*/
	bool bAttackChangeRotation = false;
	float attackRotAlpha = 0;
	float currentDamage = 0;
	float lastHitAudioPlayedTime = 0;
	
	TArray<ICombatInterface*> DamagedActors;
	
	UPROPERTY()
	USkeletalMeshComponent* CharacterMesh;
	UPROPERTY()
	UCharacterMovementComponent* CharacterMovement;
	UPROPERTY()
	UInventoryComponent* InventoryComponent;

	TWeakInterfacePtr<IPlayerInterface> CharacterState;
	

public:
	// Sets default values for this component's properties
	UCombatComponent();
	
	virtual void SetupComponent(UPlayerSettings* Settings) override;
	virtual void Dispose() override;

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void TryContinueCombo();
	AActor* GetSoftLockTarget();
	void StartSwordTrace();
	void EndSwordTrace();
	void ModifyDamage(float NewDamage);
	void ClearDamageModifier();
	void TryDamageByAbility(const FVector Position, float Damage, const float Range = 10);
	void SoftLockOff();
	void SoftLockOn();
	bool CanDamage();
	void TryDodgeSpecialAttack();

	UFUNCTION(BlueprintCallable)
	void ResetAttack();
	
	UFUNCTION(BlueprintCallable)
	bool IsPlayingAttackMontage(UAnimMontage* currentMontage);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Indication")
	UNiagaraComponent* SwordTraceVFXComponent;

	UPROPERTY(EditDefaultsOnly, Category = "Indication")
	UNiagaraComponent* SecondSwordTraceVFXComponent;
	
	UPROPERTY()
	AActor* SoftLockTarget = nullptr;
	
	int currentComboIndex = 0;
	bool bShouldContinueCombo = false;
	bool bShouldPerformAfterDodgeAttack = false;

	void TrySoftLockDash();
	UFUNCTION()
	void OnAttack();
	UFUNCTION()
	void OnBlockStart();
	UFUNCTION()
	void OnBlockEnd();

	/*
	UFUNCTION()
	void OnDodge();*/

	UFUNCTION()
	void OnEnemyDied();

	UFUNCTION()
	void OnCharacterStateChanged(ECharacterState State);

	void PlayMontage(UAnimMontage* Montage);
	void DealSwordDamage(TArray<FHitResult> Hitted, FVector WeaponTipEnd);
};
