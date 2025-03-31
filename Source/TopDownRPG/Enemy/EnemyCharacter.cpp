// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"

#include "AI/EnemyAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownRPG/Database/FEnemyData.h"
#include "TopDownRPG/UI/Enemy/EnemyLifebar.h"


// Sets default values
AEnemyCharacter::AEnemyCharacter()
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

	WeaponMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("WeaponMeshComponent");
	WeaponMeshComponent->SetCollisionProfileName("NoCollision");
}

ECharacterState AEnemyCharacter::GetState()
{
	return CurrentState;
}

void AEnemyCharacter::SetState(ECharacterState NewState)
{
	if(NewState != CurrentState)
	{
		CurrentState = NewState;
		if(OnStateChanged.IsBound())
		{
			OnStateChanged.Broadcast(CurrentState);
		}
	}
}

void AEnemyCharacter::ClearState(ECharacterState State)
{
	if(CurrentState == State)
	{
		SetState(Nothing);
	}
}

void AEnemyCharacter::OnHit(AActor* Hitter, FVector HitPosition, FVector HitVelocity)
{
	FVector Dir = HitVelocity;
	Dir.Normalize();
	FVector Right = GetActorRightVector();
	Right.Normalize();
	
	if(Dir.Dot(Right) > 0)
	{
		if(HitReactionRight && !GetCurrentMontage())
		{
			PlayAnimMontage(HitReactionRight);
		}
	}
	else if(HitReactionLeft && !GetCurrentMontage())
	{
		PlayAnimMontage(HitReactionLeft);
	}
	LaunchCharacter(HitVelocity, false, false);
	if(bIsDead)
	{
		GetMesh()->AddForceToAllBodiesBelow(HitVelocity, "pelvis", true);
	}
}

void AEnemyCharacter::OnSkillReaction(UAnimMontage* ReactionMontage)
{
	EndSwordTrace();
	PlayAnimMontage(ReactionMontage);
}

void AEnemyCharacter::Damage(float Damage)
{
	CurrentHP = FMath::Max(0, CurrentHP - Damage);

	if(UEnemyLifebar* HPBar = Cast<UEnemyLifebar>(LifeBar->GetWidget()))
	{
		HPBar->HPBar->SetPercent(CurrentHP / MaxHP);
	}

	if(CurrentHP <= 0 && !bIsDead)
	{
		Die();
	}
}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
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
			CurrentDamage = EnemyData->Damage;
			if(AEnemyAIController* EnemyAIController = Cast<AEnemyAIController>(GetController()))
			{
				EnemyAIController->SetAIData(EnemyData);
			}

			GetCharacterMovement()->MaxWalkSpeed = EnemyData->MoveSpeed;
			WeaponMeshComponent->SetStaticMesh(EnemyData->Weapoon);
			WeaponMeshComponent->AttachToComponent(GetMesh(),FAttachmentTransformRules::SnapToTargetNotIncludingScale, "weapon_r");
		}
		
		if(UEnemyLifebar* HPBar = Cast<UEnemyLifebar>(LifeBar->GetWidget()))
		{
			HPBar->HPBar->SetPercent(CurrentHP / MaxHP);
		}
	}
}

bool AEnemyCharacter::CanDamage()
{
	return !bIsDead;
}

void AEnemyCharacter::SetAirborne(bool isAirborne)
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

float AEnemyCharacter::Attack()
{
	return PlayAnimMontage(AttackAnimation);
}

void AEnemyCharacter::StartSwordTrace()
{
	bIsTracingSword = true;
}

void AEnemyCharacter::EndSwordTrace()
{
	bIsTracingSword = false;
}

// Called every frame
void AEnemyCharacter::Tick(float DeltaTime)
{
	if (bIsTracingSword)
	{
		TArray<FHitResult> OutResults;
		FVector Start = GetMesh()->GetSocketLocation("weapon_base");
		FVector End = GetMesh()->GetSocketLocation("weapon_tip");
		TArray<AActor*> ToIgnore;
		ToIgnore.Add(this);
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 10,
											   ObjectTypes,
											   false,
											   ToIgnore,
											   EDrawDebugTrace::ForDuration, OutResults, true, FLinearColor::Red,
											   FLinearColor::Green, DeltaTime * 3);

		for (FHitResult OutResult : OutResults)
		{
			if (auto* Damageable = Cast<IIDamageable>(OutResult.GetActor()))
			{
				Damageable->Damage(CurrentDamage);
				bIsTracingSword = false;
			}
		}
	}
		
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemyCharacter::Die()
{
	bIsDead = true;
	if(OnDie.IsBound())
	{
		OnDie.Broadcast();
	}

	EndSwordTrace();
	GetMesh()->SetCollisionProfileName("Ragdoll");
	GetMesh()->SetSimulatePhysics(true);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	if(LifeBar)
	{
		LifeBar->SetVisibility(false);
	}
	FTimerHandle DisappearHandle;

	GetWorldTimerManager().SetTimer(DisappearHandle, [this]()
	{
		Destroy();
	},5,false, 5);
}

