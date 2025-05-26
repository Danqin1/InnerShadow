// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_PerformAttack.generated.h"


struct FUBTT_PerformAttackMemory
{
	float ElapsedTime = 0;
	float WaitDuration = 0;
};
/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UBTT_PerformAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTT_PerformAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	virtual uint16 GetInstanceMemorySize() const override;

protected:
	UPROPERTY(EditAnywhere)
	float RotationSpeed = 1;
};
