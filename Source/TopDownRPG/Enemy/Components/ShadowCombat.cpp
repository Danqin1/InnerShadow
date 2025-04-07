// Fill out your copyright notice in the Description page of Project Settings.


#include "ShadowCombat.h"

#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownRPG/Interfaces/EnemyCombat.h"
#include "TopDownRPG/Interfaces/IDamageable.h"


// Sets default values for this component's properties
UShadowCombat::UShadowCombat()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UShadowCombat::BeginPlay()
{
	Super::BeginPlay();

	// ...
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
											   EDrawDebugTrace::ForDuration, OutResults, true, FLinearColor::Red,
											   FLinearColor::Green, 1);

		TArray<FHitResult> SecondResults;
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Right, Right, TraceRadius,
											   ObjectTypes,
											   false,
											   ToIgnore,
											   EDrawDebugTrace::ForDuration, SecondResults, true, FLinearColor::Red,
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

// Called every frame
void UShadowCombat::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

