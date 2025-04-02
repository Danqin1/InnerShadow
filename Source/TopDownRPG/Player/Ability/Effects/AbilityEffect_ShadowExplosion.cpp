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
		                                                 EDrawDebugTrace::ForDuration, OutResults, true,
		                                                 FLinearColor::Red,
		                                                 FLinearColor::Green, 3);


		Caster->GetMovementComponent()->StopMovementImmediately();
		if (CastAnimation)
		{
			Caster->PlayAnimMontage(CastAnimation);
		}
		if (VFXSystem)
		{
			auto* system = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
				GetWorld(), VFXSystem, Caster->GetActorLocation() + VFXOffset);
		}

		GetWorld()->GetTimerManager().SetTimer(AttachHandle, [this, Caster, OutResults]()
		{
			for (FHitResult OutResult : OutResults)
			{
				if (Caster)
				{
					if (auto* damageable = Cast<IIDamageable>(OutResult.GetActor()))
					{
						damageable->Damage(Damage);
						if (auto* enemy = Cast<IEnemy>(OutResult.GetActor()))
						{
							enemy->OnHit(Caster, OutResult.Location,
							             (OutResult.Location - Caster->GetActorLocation()) *
							             PushEnemiesStrength + FVector::UpVector * PushEnemiesStrength);
						}
					}
				}
			}
		}, ImpactDelay, false, ImpactDelay);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			Destroy();
		}, Duration, false, Duration);
	}
}
