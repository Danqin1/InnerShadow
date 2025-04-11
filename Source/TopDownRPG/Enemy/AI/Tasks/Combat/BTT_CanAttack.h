// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTT_CanAttack.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UBTT_CanAttack : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTT_CanAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
