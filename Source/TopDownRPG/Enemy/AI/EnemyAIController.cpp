// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Hearing.h"
#include "Perception/AISenseConfig_Sight.h"


// Sets default values
AEnemyAIController::AEnemyAIController()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>("PerceptionComponent");

	SetPerceptionComponent(*AIPerceptionComponent);
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseConfigSight->PeripheralVisionAngleDegrees = 90.f;
	AISenseConfigSight->AutoSuccessRangeFromLastSeenLocation = 10;
	AISenseConfigSight->SetMaxAge((1.f));

	AISenseConfigHearing = CreateDefaultSubobject<UAISenseConfig_Hearing>("SenseHearing");
	AISenseConfigHearing->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigHearing->DetectionByAffiliation.bDetectFriendlies = true;
	AISenseConfigHearing->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseConfigHearing->SetMaxAge(1);

	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->ConfigureSense(*AISenseConfigHearing);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
}

// Called when the game starts or when spawned
void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GetPerceptionComponent()->OnTargetPerceptionUpdated.RemoveDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	GetPerceptionComponent()->OnTargetPerceptionForgotten.RemoveDynamic(this, &AEnemyAIController::OnTargetForgotten);
		
	GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIController::OnTargetPerceptionUpdated);
	GetPerceptionComponent()->OnTargetPerceptionForgotten.AddDynamic(this, &AEnemyAIController::OnTargetForgotten);
}

void AEnemyAIController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if(ControllerCharacter)
	{
		ControllerCharacter->OnAirborne.RemoveDynamic(this, &AEnemyAIController::OnCharacterAirborne);
		if(auto* StateCharacter = Cast<IICharacterState>(ControllerCharacter))
		{
			StateCharacter->OnStateChanged.RemoveDynamic(this, &AEnemyAIController::OnStateChanged);
		}
	}
	Super::EndPlay(EndPlayReason);
}

void AEnemyAIController::OnPossess(APawn* InPawn)
{
	if(auto* EnemyCharacter = Cast<AEnemyCharacterBase>(InPawn))
	{
		ControllerCharacter = EnemyCharacter;
		EnemyCharacter->OnAirborne.AddDynamic(this, &AEnemyAIController::OnCharacterAirborne);
		if(auto* StateCharacter = Cast<IICharacterState>(EnemyCharacter))
		{
			StateCharacter->OnStateChanged.AddDynamic(this, &AEnemyAIController::OnStateChanged);
		}
		StartLocation = EnemyCharacter->GetActorLocation();
	}
	
	Super::OnPossess(InPawn);
}

ETeamAttitude::Type AEnemyAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	return ETeamAttitude::Friendly;
}

void AEnemyAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if(!Cast<AEnemyCharacterBase>(Actor))
	{
		if(Stimulus.WasSuccessfullySensed())
		{
			GetBlackboardComponent()->SetValueAsObject(Param_TargetPlayer, Actor);
		}
	}
}

void AEnemyAIController::OnTargetForgotten(AActor* Actor)
{
	GetBlackboardComponent()->SetValueAsObject(Param_TargetPlayer, nullptr);
}

void AEnemyAIController::OnCharacterAirborne(bool isAirborne)
{
	GetBlackboardComponent()->SetValueAsBool(Param_Airborne, isAirborne);
}

void AEnemyAIController::OnStateChanged(ECharacterState State)
{
	GetBlackboardComponent()->SetValueAsEnum(Param_State, State);
}

// Called every frame
void AEnemyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemyAIController::SetAIData(FEnemyData* Data)
{
	if (AISenseConfigSight && AIPerceptionComponent)
	{
		AISenseConfigSight->SightRadius = Data->ViewRange;
		AISenseConfigSight->PeripheralVisionAngleDegrees = Data->FieldOfView;
		AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	}

	if(AISenseConfigHearing && AIPerceptionComponent)
	{
		AISenseConfigHearing->HearingRange = Data->HearRange;
		AIPerceptionComponent->ConfigureSense(*AISenseConfigHearing);
	}

	RunBehaviorTree(Data->MainBehaviourTree);
}

