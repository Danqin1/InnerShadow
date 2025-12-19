// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacterBase.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/Database/FEnemyData.h"
#include "TopDownRPG/Enemy/AI/EnemyAIController.h"
#include "TopDownRPG/Interfaces/PlayerInterface.h"
#include "TopDownRPG/Player/Components/PlayerStatsComponent.h"
#include "TopDownRPG/UI/Enemy/EnemyLifebar.h"


// Sets default values
AEnemyCharacterBase::AEnemyCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	LifeBar = CreateDefaultSubobject<UWidgetComponent>("Life Bar");
	LifeBar->SetupAttachment(GetRootComponent());

	if(GetMesh())
	{
		GetMesh()->SetCollisionProfileName("CharacterMesh");
	}

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

ECharacterState AEnemyCharacterBase::GetState()
{
	return CurrentState;
}

void AEnemyCharacterBase::SetState(ECharacterState NewState, bool force)
{
	if(NewState != CurrentState && (CanChangeToState(NewState) || force))
	{
		CurrentState = NewState;
		if(OnStateChanged.IsBound())
		{
			OnStateChanged.Broadcast(CurrentState);
		}
	}
}

bool AEnemyCharacterBase::CanChangeToState(ECharacterState NewState)
{
	switch (NewState)
	{
	case Nothing:
		{
			switch (CurrentState) {
			case Attacking:
			case Interaction:
			case Cutscene:
			case Dead:
			case Skill:
			case Frozen:
				return false;
			default: return true;
			}
		}
	case Chasing:
		{
			switch (CurrentState) {
			case Attacking:
			case Dead:
			case Skill:
			case Frozen:
				return false;
			default: return true;
			}
		}
	case Dead:
		return true;
	case Frozen:
		return CurrentState != Dead;
		break;
		default: return true;
	}
}

void AEnemyCharacterBase::ClearState(ECharacterState State)
{
	if(CurrentState == State)
	{
		SetState(Nothing, true);
	}
}

void AEnemyCharacterBase::OnSkillReaction(UAnimMontage* ReactionMontage)
{
	Combat->OnSkillReaction(ReactionMontage);
}

bool AEnemyCharacterBase::Hit(AActor* Hitter, FVector HitPosition, FVector HitVelocity,float Damage, bool canCrushBlock, bool withReaction, UAnimMontage* reaction)
{
	if (GetState() == Block)
	{
		if (!canCrushBlock) return false;
		ClearState(Block);
	}
	
	CurrentHP = FMath::Max(0, CurrentHP - Damage);

	if(UEnemyLifebar* HPBar = Cast<UEnemyLifebar>(LifeBar->GetWidget()))
	{
		HPBar->HPBar->SetPercent(CurrentHP / MaxHP);
	}

	Combat->OnDamaged(CurrentHP);

	if(CurrentHP <= 0 && GetState() != Dead)
	{
		Die();
	}
	else
	{
		Combat->OnHit(Hitter, HitPosition, HitVelocity, withReaction, reaction);
	}
	return true;
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
	if(Data)
	{
		const FString ContextString(TEXT("Enemy data Context"));
		FEnemyData* EnemyData = Data->FindRow<FEnemyData>(EnemyDataName, ContextString, true);
		if(EnemyData)
		{
			GetMesh()->SetSkeletalMesh(EnemyData->SkeletalMesh);
			GetMesh()->SetAnimInstanceClass(EnemyData->AnimBP);

			MaxHP = EnemyData->MaxHP;
			CurrentHP = MaxHP;

			if(AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController()))
			{
				EnemyAIController->SetAIData(EnemyData);
			}

			GetCharacterMovement()->MaxWalkSpeed = EnemyData->MoveSpeed;
		}

		if (Combat)
		{
			Combat->Setup(EnemyData);
		}
		
		if(UEnemyLifebar* HPBar = Cast<UEnemyLifebar>(LifeBar->GetWidget()))
		{
			HPBar->HPBar->SetPercent(CurrentHP / MaxHP);
		}
	}
}

bool AEnemyCharacterBase::CanDamage()
{
	return GetState() != Dead;
}

void AEnemyCharacterBase::SetAirborne(bool isAirborne)
{
	if(OnAirborne.IsBound())
	{
		OnAirborne.Broadcast(isAirborne);
	}
	if(isAirborne)
	{
		GetCharacterMovement()->GravityScale = 0;
	}
	else
	{
		GetCharacterMovement()->GravityScale = 1;
	}
}

float AEnemyCharacterBase::PerformAttack()
{
	Combat->Attack();
	return 1;
}

bool AEnemyCharacterBase::CanAttack()
{
	return Combat->CanAttack();
}

void AEnemyCharacterBase::StartTraceAttack()
{
	Combat->StartAttackTrace();
}

void AEnemyCharacterBase::EndTraceAttack()
{
	Combat->EndAttackTrace();
}

void AEnemyCharacterBase::ChangeMovementMode(EMovementType NewMovement)
{
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	const FString ContextString(TEXT("Enemy data Context"));
	FEnemyData* EnemyData = Data->FindRow<FEnemyData>(EnemyDataName, ContextString, true);
	
	if (Movement && EnemyData)
	{
		switch (NewMovement) {
		case Walk:
			Movement->MaxWalkSpeed = EnemyData->MoveSpeed;
			break;
		case Run:
			Movement->MaxWalkSpeed = EnemyData->RunSpeed;
			break;
		}
	}
}

void AEnemyCharacterBase::Freeze(bool isFrozen)
{
	if (isFrozen)
	{
		GetMesh()->bPauseAnims = true;
		GetMovementComponent()->StopActiveMovement();
		SetState(Frozen);
		
	}
	else
	{
		GetMesh()->bPauseAnims = false;
		ClearState(Frozen);
	}
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacterBase::Die()
{
	SetState(Dead);
	if(OnDie.IsBound())
	{
		OnDie.Broadcast();
	}

	EndTraceAttack();
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	if(LifeBar)
	{
		LifeBar->SetVisibility(false);
	}
	FTimerHandle DisappearHandle;

	const FString ContextString(TEXT("Enemy data Context"));
	FEnemyData* EnemyData = Data->FindRow<FEnemyData>(EnemyDataName, ContextString, true);
	if(EnemyData)
	{
		if (AActor* player =  UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		{
			if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(player))
			{
				PlayerInterface->AddPrize(EnemyData->KillPrize);
			}
		}
	}

	GetWorldTimerManager().SetTimer(DisappearHandle, [this]()
	{
		Destroy();
	},5,false, 5);
}

