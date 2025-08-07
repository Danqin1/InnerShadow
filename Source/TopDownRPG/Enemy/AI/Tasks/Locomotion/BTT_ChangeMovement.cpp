// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChangeMovement.h"

#include "AIController.h"
#include "TopDownRPG/Interfaces/EnemyInterface.h"

UBTT_ChangeMovement::UBTT_ChangeMovement()
{
	NodeName = "ChangeMovement";
}

EBTNodeResult::Type UBTT_ChangeMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IEnemyInterface* Enemy = Cast<IEnemyInterface>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Enemy->ChangeMovementMode(NewMovement);
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
