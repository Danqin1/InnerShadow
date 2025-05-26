// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "TopDownRPG/TopDownRPG.h"
#include "BTT_ChangeMovement.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UBTT_ChangeMovement : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	UBTT_ChangeMovement();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
protected:
	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMovementType> NewMovement;
};
