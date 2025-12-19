// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCombatBase.h"

#include "GameFramework/Character.h"

UEnemyCombatBase::UEnemyCombatBase()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UEnemyCombatBase::BeginPlay()
{
	Super::BeginPlay();
}

void UEnemyCombatBase::TraceAttack()
{
}

void UEnemyCombatBase::Setup(FEnemyData* Data)
{
	CurrentDamage = Data->Damage;
}

void UEnemyCombatBase::OnHit(AActor* Hitter, FVector HitPosition, FVector HitVelocity, bool withReaction, UAnimMontage* reaction)
{
	if (!withReaction) return;
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		Character->PlayAnimMontage(reaction ? reaction : HitReactionFront);
	}
}

void UEnemyCombatBase::OnDamaged(float CurrentHealth)
{
}

void UEnemyCombatBase::StartAttackTrace()
{
	bIsTracingAttack = true;
}

void UEnemyCombatBase::EndAttackTrace()
{
	bIsTracingAttack = false;
}

void UEnemyCombatBase::OnSkillReaction(UAnimMontage* ReactionMontage)
{
	EndAttackTrace();
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		Character->PlayAnimMontage(ReactionMontage);
	}
}

void UEnemyCombatBase::Attack()
{
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		Character->PlayAnimMontage(AttackAnimation);
	}
}

void UEnemyCombatBase::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	TraceAttack();
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

