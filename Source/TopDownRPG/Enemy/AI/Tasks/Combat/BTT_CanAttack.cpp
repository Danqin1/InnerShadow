// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_CanAttack.h"

#include "AIController.h"
#include "TopDownRPG/Interfaces/CombatInterface.h"

UBTT_CanAttack::UBTT_CanAttack()
{
	NodeName = TEXT("Can attack");
}

EBTNodeResult::Type UBTT_CanAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(ICombatInterface* Combat = Cast<ICombatInterface>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		return Combat->CanAttack() ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
