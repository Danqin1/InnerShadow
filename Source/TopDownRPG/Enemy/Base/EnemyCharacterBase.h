// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyCombatBase.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "TopDownRPG/Interfaces/CombatInterface.h"
#include "TopDownRPG/Interfaces/EnemyInterface.h"
#include "TopDownRPG/Interfaces/DamageableInterface.h"
#include "EnemyCharacterBase.generated.h"

UCLASS()
class TOPDOWNRPG_API AEnemyCharacterBase : public ACharacter, public IDamageableInterface, public IEnemyInterface, public ICombatInterface
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
	
	ECharacterState CurrentState = Nothing;
	
	float MaxHP = 0;
	float CurrentHP = 0;

	void Die();
	
public:
	FBoolEvent OnAirborne;
	
	AEnemyCharacterBase();

	virtual ECharacterState GetState() override;
	virtual void SetState(ECharacterState NewState) override;
	virtual void ClearState(ECharacterState State) override;
	
	virtual void OnHit(AActor* Hitter, FVector HitPosition, FVector HitVelocity) override;
	virtual void OnSkillReaction(UAnimMontage* ReactionMontage) override;
	virtual void Damage(float Damage) override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual bool CanDamage() override;
	void SetAirborne(bool isAirborne);
	virtual float Attack() override;
	virtual bool CanAttack() override;
	virtual void StartTraceAttack() override;
	virtual void EndTraceAttack() override;
	virtual void ChangeMovementMode(EMovementType NewMovement) override;
};
