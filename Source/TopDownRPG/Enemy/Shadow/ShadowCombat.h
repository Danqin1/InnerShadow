// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/Enemy/Base/EnemyCombatBase.h"
#include "ShadowCombat.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API UShadowCombat : public UEnemyCombatBase
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UShadowCombat();

protected:
	UPROPERTY(EditAnywhere, Category="Combat")
	FName SocketLeft = "weapon_l";

	UPROPERTY(EditAnywhere, Category="Combat")
	FName SocketRight = "weapon_r";

	UPROPERTY(EditAnywhere, Category="Combat")
	float TraceRadius = 20;
	
	UPROPERTY()
	USkeletalMeshComponent* Mesh;
	

	// Called when the game starts
	virtual void BeginPlay() override;

	virtual void TraceAttack() override;
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	virtual void OnHit(AActor* Hitter, FVector HitPosition, FVector HitVelocity, bool withReaction, UAnimMontage* reaction) override;
};
