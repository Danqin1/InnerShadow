// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetState.h"

#include "AIController.h"
#include "TopDownRPG/Interfaces/ICharacterState.h"

EBTNodeResult::Type UBTT_SetState::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(IICharacterState* Character = Cast<IICharacterState>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		Character->SetState(NewState);
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return EBTNodeResult::Succeeded;
	}
	return EBTNodeResult::Failed;
}
