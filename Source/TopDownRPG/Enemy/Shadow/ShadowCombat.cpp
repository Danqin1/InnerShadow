// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowCombat.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownRPG/Interfaces/EnemyCombat.h"
#include "TopDownRPG/Interfaces/ICharacterState.h"
#include "TopDownRPG/Interfaces/IDamageable.h"

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
		TArray<FHitResult> OutResults;
		FVector Left = Mesh->GetSocketLocation(SocketLeft);
		FVector Right = Mesh->GetSocketLocation(SocketRight);
		TArray<AActor*> ToIgnore;
		
		ToIgnore.Add(GetOwner());
		
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Left, Left, TraceRadius,
											   ObjectTypes,
											   false,
											   ToIgnore,
											   EDrawDebugTrace::None, OutResults, true, FLinearColor::Red,
											   FLinearColor::Green, 1);

		TArray<FHitResult> SecondResults;
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Right, Right, TraceRadius,
											   ObjectTypes,
											   false,
											   ToIgnore,
											   EDrawDebugTrace::None, SecondResults, true, FLinearColor::Red,
											   FLinearColor::Green, 1);

		OutResults.Append(SecondResults);

		for (FHitResult OutResult : OutResults)
		{
			if (auto* Damageable = Cast<IIDamageable>(OutResult.GetActor()))
			{
				if (!Cast<IEnemyCombat>(OutResult.GetActor()))
				{
					Damageable->Damage(CurrentDamage);
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

void UShadowCombat::OnHit(AActor* Hitter, FVector HitPosition, FVector HitVelocity)
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

