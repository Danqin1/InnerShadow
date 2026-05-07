// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCombatBase.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "TopDownRPG/Interfaces/CombatInterface.h"
#include "TopDownRPG/Interfaces/EnemyInterface.h"
#include "EnemyCharacterBase.generated.h"

class UNiagaraComponent;

UCLASS()
class TOPDOWNRPG_API AEnemyCharacterBase : public ACharacter, public IEnemyInterface, public ICombatInterface
{
private:
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditDefaultsOnly, Category="Data")
	FName EnemyDataName = "DefaultEnemy";
	
	UPROPERTY(EditDefaultsOnly, Category="Data")
	UDataTable* Data;

	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* LifeBar;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UEnemyCombatBase* Combat;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="VFX")
	UNiagaraComponent* ChargeAttackVFX;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="VFX")
	FName ChargeAttackVFXSocketName = "weapon_r";
	
	ECharacterState CurrentState = Nothing;
	
	float MaxHP = 0;
	float CurrentHP = 0;

	void Die();
	
public:
	FBoolEvent OnAirborne;
	
	AEnemyCharacterBase();

	UFUNCTION(BlueprintCallable)
	virtual ECharacterState GetState() override;
	
	UFUNCTION(BlueprintCallable)
	virtual void SetState(ECharacterState NewState, bool force = false) override;

	bool CanChangeToState(ECharacterState NewState);
	
	UFUNCTION(BlueprintCallable)
	virtual void ClearState(ECharacterState State) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void OnSkillReaction(UAnimMontage* ReactionMontage) override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool Hit(AActor* Hitter, FVector HitPosition, FVector HitVelocity, float Damage, bool canCrushBlock, bool withReaction = true, UAnimMontage* reaction = nullptr) override;
	
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	virtual bool CanDamage() override;
	
	UFUNCTION(BlueprintCallable)
	void SetAirborne(bool isAirborne);

	UFUNCTION(BlueprintCallable)
	virtual float PerformAttack() override;
	
	UFUNCTION(BlueprintCallable)
	virtual bool CanAttack() override;

	UFUNCTION(BlueprintCallable)
	virtual void StartTraceAttack() override;

	UFUNCTION(BlueprintCallable)
	virtual void EndTraceAttack() override;

	UFUNCTION(BlueprintCallable)
	virtual void ChangeMovementMode(EMovementType NewMovement) override;

	UFUNCTION(BlueprintCallable)
	virtual void Freeze(bool isFrozen) override;
	
	UFUNCTION(BlueprintCallable)
	virtual void ChargeAttack(bool start) override;
};
