// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityEffect_ShadowExplosion.h"

#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/Character.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownRPG/Interfaces/Enemy.h"
#include "TopDownRPG/Interfaces/IDamageable.h"


// Sets default values
AAbilityEffect_ShadowExplosion::AAbilityEffect_ShadowExplosion()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

// Called when the game starts or when spawned
void AAbilityEffect_ShadowExplosion::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AAbilityEffect_ShadowExplosion::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AAbilityEffect_ShadowExplosion::Activate(ACharacter* Caster)
{
	Super::Activate(Caster);

	if (Caster)
	{
		FTimerHandle AttachHandle;


		Caster->GetMovementComponent()->StopMovementImmediately();
		if (CastAnimation)
		{
			Caster->PlayAnimMontage(CastAnimation);
		}
		if (VFXSystem)
		{
			auto* system = UNiagaraFunctionLibrary::SpawnSystemAttached(VFXSystem, Caster->GetMesh(), "root",
			                                                            FVector(0, 0, 50), FRotator::ZeroRotator,
			                                                            EAttachLocation::Type::SnapToTarget, true);
		}

		GetWorld()->GetTimerManager().SetTimer(AttachHandle, [this, Caster]()
		{
			TArray<FHitResult> OutResults;
			FVector Start = Caster->GetActorLocation();
			TArray<AActor*> ToIgnore;
			ToIgnore.Add(Caster);
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

			UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, Start, ImpactRadius,
			                                                 ObjectTypes,
			                                                 false,
			                                                 ToIgnore,
			                                                 EDrawDebugTrace::None, OutResults, true,
			                                                 FLinearColor::Red,
			                                                 FLinearColor::Green, 3);

			TArray<AActor*> Hitted;
			for (FHitResult OutResult : OutResults)
			{
				if (Caster)
				{
					if (auto* damageable = Cast<IIDamageable>(OutResult.GetActor()))
					{
						if (auto* enemy = Cast<IEnemy>(OutResult.GetActor()))
						{
							if (!Hitted.Contains(OutResult.GetActor()))
							{
								damageable->Damage(Damage);
								enemy->OnHit(Caster, OutResult.Location, FVector::UpVector * PushEnemiesStrength);
								Hitted.Add(OutResult.GetActor());
							}
							//(OutResult.Location - Caster->GetActorLocation()) *
							//PushEnemiesStrength + FVector::UpVector * PushEnemiesStrength);
						}
					}
				}
			}

			OnEffectFinished();
		}, ImpactDelay, false, ImpactDelay);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			Destroy();
		}, Duration, false, Duration);
	}
}
