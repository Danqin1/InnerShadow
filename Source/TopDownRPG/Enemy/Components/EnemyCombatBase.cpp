// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCombatBase.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownRPG/Interfaces/ICharacterState.h"


// Sets default values for this component's properties
UEnemyCombatBase::UEnemyCombatBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemyCombatBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void UEnemyCombatBase::TraceAttack()
{
	/*if (bIsTracingSword)
	{
		TArray<FHitResult> OutResults;
		FVector Start = GetMesh()->GetSocketLocation("weapon_base");
		FVector End = GetMesh()->GetSocketLocation("weapon_tip");
		TArray<AActor*> ToIgnore;
		ToIgnore.Add(this);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 10,
											   ObjectTypes,
											   false,
											   ToIgnore,
											   EDrawDebugTrace::ForDuration, OutResults, true, FLinearColor::Red,
											   FLinearColor::Green, DeltaTime * 3);

		for (FHitResult OutResult : OutResults)
		{
			if (auto* Damageable = Cast<IIDamageable>(OutResult.GetActor()))
			{
				Damageable->Damage(CurrentDamage);
				bIsTracingSword = false;
			}
		}
	}*/
}

void UEnemyCombatBase::Setup(FEnemyData* Data)
{
	CurrentDamage = Data->Damage;
}

void UEnemyCombatBase::OnHit(AActor* Hitter, FVector HitPosition, FVector HitVelocity)
{
	FVector Dir = HitVelocity;
	Dir.Normalize();
	FVector Right = GetOwner()->GetActorRightVector();
	Right.Normalize();
	
	
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		Character->StopAnimMontage();
		Character->LaunchCharacter(HitVelocity, false, false);

		if (IICharacterState* state = Cast<IICharacterState>(GetOwner()))
		{
			if(state->GetState() == Dead)
			{
				Character->GetMesh()->AddForceToAllBodiesBelow(HitVelocity, "pelvis", true);
			}
		}
		else
		{
			if(Dir.Dot(Right) > 0)
			{
				if(HitReactionRight && !Character->GetCurrentMontage())
				{
					Character->PlayAnimMontage(HitReactionRight);
				}
			}
			else if(HitReactionLeft && !Character->GetCurrentMontage())
			{
				Character->PlayAnimMontage(HitReactionLeft);
			}
		}
	}
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


// Called every frame
void UEnemyCombatBase::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	TraceAttack();
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

