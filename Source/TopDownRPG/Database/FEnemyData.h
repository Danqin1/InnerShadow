#pragma once

#include "BehaviorTree/BehaviorTree.h"
#include "Engine/DataTable.h"
#include "TopDownRPG/Enemy/FKillPrize.h"
#include "FEnemyData.generated.h"

USTRUCT(BlueprintType)
struct FEnemyData : public FTableRowBase
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMesh* SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UAnimInstance> AnimBP;

	UPROPERTY(EditAnywhere, Category="Combat")
	UStaticMesh* Weapoon;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float MaxHP = 100;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float Damage = 5;
	
	UPROPERTY(EditAnywhere, Category="Combat")
	float MoveSpeed = 500;

	UPROPERTY(EditAnywhere, Category="AI")
	float ViewRange = 1000;

	UPROPERTY(EditAnywhere, Category="AI")
	float HearRange = 1000;

	UPROPERTY(EditAnywhere, Category="AI")
	float FieldOfView = 90;

	UPROPERTY(EditAnywhere, Category="AI")
	UBehaviorTree* MainBehaviourTree;

	UPROPERTY(EditAnywhere, Category= "General")
	FKillPrize KillPrize;
};
