// Fill out your copyright notice in the Description page of Project Settings.


#include "BTT_PerformAttack.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TopDownRPG/Interfaces/CombatInterface.h"

UBTT_PerformAttack::UBTT_PerformAttack()
{
	NodeName = TEXT("Attack");
	bNotifyTick = true;
	bTickIntervals = 0.1;
}

void UBTT_PerformAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FUBTT_PerformAttackMemory* MyMemory = reinterpret_cast<FUBTT_PerformAttackMemory*>(NodeMemory);
	if(MyMemory->ElapsedTime >= MyMemory->WaitDuration)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		AActor* target = Cast<AActor>(OwnerComp.GetBlackboardComponent()->GetValueAsObject( BlackboardKey.SelectedKeyName ));
		if (target)
		{
			if(auto Pawn = OwnerComp.GetAIOwner()->GetPawn())
			{
				FVector TargetLocation = target->GetActorLocation();
				FRotator CurrentRotation = Pawn->GetActorRotation();
				FVector PawnLocation = Pawn->GetActorLocation();
				FVector Direction = TargetLocation - PawnLocation;
				Direction.Z = 0;
				FRotator TargetRotation = UKismetMathLibrary::MakeRotFromX(Direction);
				FRotator SmoothRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaSeconds, RotationSpeed);
			
				Pawn->SetActorRotation(SmoothRotation);
			}
		}
		
		MyMemory->ElapsedTime += DeltaSeconds;
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}
}

uint16 UBTT_PerformAttack::GetInstanceMemorySize() const
{
	return sizeof(FUBTT_PerformAttackMemory);
}

EBTNodeResult::Type UBTT_PerformAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if(ICombatInterface* Combat = Cast<ICombatInterface>(OwnerComp.GetAIOwner()->GetPawn()))
	{
		FUBTT_PerformAttackMemory* MyMemory = reinterpret_cast<FUBTT_PerformAttackMemory*>(NodeMemory);
		
		MyMemory->WaitDuration = Combat->PerformAttack();
		MyMemory->ElapsedTime = 0;
		
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
		return EBTNodeResult::InProgress;
	}
	return EBTNodeResult::Failed;
}
