// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_SetRandomTargetLocation.h"

#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TopDownRPG/Enemy/AI/EnemyAIController.h"

UBTT_SetRandomTargetLocation::UBTT_SetRandomTargetLocation()
{
	NodeName = "Set Random Location";
}

EBTNodeResult::Type UBTT_SetRandomTargetLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(AEnemyAIController* const aiController = Cast<AEnemyAIController>(OwnerComp.GetAIOwner()))
	{
		TObjectPtr<APawn> pawn = aiController->GetPawn();
		if(pawn)
		{
			FVector const origin = aiController->StartLocation;

			// get navmesh
			if(UNavigationSystemV1* const NavSys = UNavigationSystemV1::GetCurrent(GetWorld()))
			{
				FNavLocation NavLocation;
				if(NavSys->GetRandomPointInNavigableRadius(origin, RandomPointMaxDistance, NavLocation))
				{
					OwnerComp.GetBlackboardComponent()->SetValueAsVector(GetSelectedBlackboardKey(), NavLocation.Location);
				}

				FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				return EBTNodeResult::Succeeded;
			}
		}
	}
	return EBTNodeResult::Failed;
}
