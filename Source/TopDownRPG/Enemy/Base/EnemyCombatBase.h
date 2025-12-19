// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TopDownRPG/Database/FEnemyData.h"
#include "EnemyCombatBase.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API UEnemyCombatBase : public UActorComponent
{
	GENERATED_BODY()

	
public:
	// Sets default values for this component's properties
	UEnemyCombatBase();

protected:
	bool bIsTracingAttack = false;
	UPROPERTY(BlueprintReadOnly)
	float CurrentDamage = 0;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* HitReactionLeft;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* HitReactionRight;
	
	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* HitReactionFront;

	UPROPERTY(EditDefaultsOnly, Category="Combat")
	UAnimMontage* AttackAnimation;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void TraceAttack();
	
public:
	void Setup(FEnemyData* Data);
	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	virtual void OnHit(AActor* Hitter, FVector HitPosition, FVector HitVelocity, bool withReaction, UAnimMontage* reaction);

	virtual void OnDamaged(float CurrentHealth);

	void StartAttackTrace();
	void EndAttackTrace();
	virtual void OnSkillReaction(UAnimMontage* ReactionMontage);
	virtual void Attack();
	virtual bool CanAttack(){return true;}
};
