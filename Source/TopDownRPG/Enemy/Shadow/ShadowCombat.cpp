// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowCombat.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownRPG/Interfaces/CombatInterface.h"
#include "TopDownRPG/Interfaces/EnemyInterface.h"

UShadowCombat::UShadowCombat()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UShadowCombat::BeginPlay()
{
	Super::BeginPlay();

	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		Mesh = Character->GetMesh();
	}
}

void UShadowCombat::TraceAttack()
{
	if (bIsTracingAttack)
	{
		if (IEnemyInterface* EnemyInterface = Cast<IEnemyInterface>(GetOwner()))
		{
			if (EnemyInterface->GetState() == Dead)
			{
				return;
			}
		}
		
		TArray<FHitResult> OutResults;
		FVector Left = Mesh->GetSocketLocation(SocketLeft);
		FVector Right = Mesh->GetSocketLocation(SocketRight);
		TArray<AActor*> ToIgnore;
		
		ToIgnore.Add(GetOwner());
		
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		/*
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Left, Left, TraceRadius,
											   ObjectTypes,
											   false,
											   ToIgnore,
											   EDrawDebugTrace::None, OutResults, true, FLinearColor::Red,
											   FLinearColor::Green, 1);
											   */

		TArray<FHitResult> SecondResults;
		EDrawDebugTrace::Type draw = bDrawDebug ? EDrawDebugTrace::ForDuration : EDrawDebugTrace::None;
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Right, Right, TraceRadius,
											   ObjectTypes,
											   false,
											   ToIgnore,
												draw, SecondResults, true, FLinearColor::Red,
											   FLinearColor::Green, .5f);

		OutResults.Append(SecondResults);

		for (FHitResult OutResult : OutResults)
		{
			if (OutResult.GetActor() != GetOwner())
			{
				if (auto* Combat = Cast<ICombatInterface>(OutResult.GetActor()))
				{
					Combat->Hit(GetOwner(), OutResult.ImpactPoint, FVector::One(), CurrentDamage, false);
					bIsTracingAttack = false;
				}
			}
		}
	}
}

void UShadowCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UShadowCombat::OnHit(AActor* Hitter, FVector HitPosition, FVector HitVelocity, bool withReaction, UAnimMontage* reaction)
{
	FVector Dir = HitVelocity;
	Dir.Normalize();
	FVector Right = GetOwner()->GetActorRightVector();
	Right.Normalize();
	
	
	if (ACharacter* Character = Cast<ACharacter>(GetOwner()))
	{
		Character->StopAnimMontage();
		Character->LaunchCharacter(HitVelocity, false, false);

		if (IEnemyInterface* state = Cast<IEnemyInterface>(GetOwner()))
		{
			if(state->GetState() == Dead)
			{
				Character->GetMesh()->AddForceToAllBodiesBelow(HitVelocity, "pelvis", true);
			}
		}
		else if (withReaction)
		{
			if(Dir.Dot(Right) > 0)
			{
				if(HitReactionRight && !Character->GetCurrentMontage())
				{
					Character->PlayAnimMontage(reaction ? reaction : HitReactionRight);
				}
			}
			else if(HitReactionLeft && !Character->GetCurrentMontage())
			{
				Character->PlayAnimMontage(reaction ? reaction : HitReactionLeft);
			}
		}
	}
}

