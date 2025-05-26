// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_ChangeMovement.h"

#include "AIController.h"
#include "TopDownRPG/Interfaces/Enemy.h"

UBTT_ChangeMovement::UBTT_ChangeMovement()
{
	NodeName = "ChangeMovement";
}

EBTNodeResult::Type UBTT_ChangeMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (IEnemy* Enemy = Cast<IEnemy>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Enemy->ChangeMovementMode(NewMovement);
	}
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
