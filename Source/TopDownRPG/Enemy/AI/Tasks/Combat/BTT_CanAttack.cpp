// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CanAttack.h"

#include "AIController.h"
#include "TopDownRPG/Interfaces/EnemyCombat.h"

UBTT_CanAttack::UBTT_CanAttack()
{
	NodeName = TEXT("Can attack");
}

EBTNodeResult::Type UBTT_CanAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(IEnemyCombat* Combat = Cast<IEnemyCombat>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		return Combat->CanAttack() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
