// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityEffect_ShadowBlast.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/SkeletalMeshSocket.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownRPG/DevDebug.h"
#include "TopDownRPG/Enemy/EnemyCharacter.h"
#include "TopDownRPG/Interfaces/Combat.h"
#include "TopDownRPG/Interfaces/Enemy.h"
#include "TopDownRPG/Player/RPGCharacter.h"


// Sets default values
AAbilityEffect_ShadowBlast::AAbilityEffect_ShadowBlast()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AAbilityEffect_ShadowBlast::BeginPlay()
{
	Super::BeginPlay();
}

void AAbilityEffect_ShadowBlast::Activate(ACharacter* Caster)
{
	if (Caster)
	{
		if (ARPGCharacter* CasterCharacter = Cast<ARPGCharacter>(Caster))
		{
			FTimerHandle AttachHandle;

			TArray<FHitResult> OutResults;
			FVector Start = Caster->GetActorLocation();
			FVector End = Start + Caster->GetActorForwardVector() * TraceLength;
			TArray<AActor*> ToIgnore;
			ToIgnore.Add(Caster);
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

			UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, TraceRadius,
			                                                 ObjectTypes,
			                                                 false,
			                                                 ToIgnore,
			                                                 EDrawDebugTrace::None, OutResults, true,
			                                                 FLinearColor::Red,
			                                                 FLinearColor::Green, 3);


			Caster->GetMovementComponent()->StopMovementImmediately();
			if (BlastAnimation)
			{
				Caster->PlayAnimMontage(BlastAnimation);
			}
			if (VFXSystem)
			{
				auto* system = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), VFXSystem, CasterCharacter->GetActorLocation() + VFXOffset);
				system->SetVectorParameter("Direction", Caster->GetActorForwardVector().GetSafeNormal());
			}

			CasterCharacter->CombatComponent->ModifyDamage(Damage);

			GetWorld()->GetTimerManager().SetTimer(AttachHandle, [this, CasterCharacter, OutResults]()
			{
				for (FHitResult OutResult : OutResults)
				{
					if (IEnemy* Enemy = Cast<IEnemy>(OutResult.GetActor()))
					{
						if (BlastReactionAnimation)
						{
							Enemy->OnSkillReaction(BlastReactionAnimation);
						}

						if (CasterCharacter)
						{
							if (auto* damageable = Cast<IIDamageable>(OutResult.GetActor()))
							{
								damageable->Damage(Damage);
								if (auto* enemy = Cast<IEnemy>(OutResult.GetActor()))
								{
									enemy->OnHit(CasterCharacter, OutResult.Location,
									             (OutResult.Location - CasterCharacter->GetActorLocation()) *
									             PushEnemiesStrength);
								}
							}
						}
					}
				}
			}, ImpactDelay, false, ImpactDelay);

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, CasterCharacter]()
			{
				if (CasterCharacter)
				{
					CasterCharacter->CombatComponent->ClearDamageModifier();
				}

				Destroy();
			}, Duration, false, Duration);
		}
	}
}
