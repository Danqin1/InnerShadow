// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"

#include "AbilityComponent.h"
#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "TopDownRPG/Interfaces/Enemy.h"
#include "TopDownRPG/Player/RPGCharacter.h"

UCombatComponent::UCombatComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	SwordTraceVFXComponent = CreateDefaultSubobject<UNiagaraComponent>("SwordTraceVFXComponent");
}

void UCombatComponent::SetupComponent()
{
	if (auto* Character = Cast<ARPGCharacter>(GetOwner()))
	{
		CharacterMesh = Character->GetMesh();
		CharacterMovement = Character->GetCharacterMovement();
		InventoryComponent = Character->InventoryComponent;
		ClearDamageModifier();
		CharacterState = Cast<IICharacterState>(Character);
		check(CharacterState.Get());

		CharacterState->OnStateChanged.AddDynamic(this, &UCombatComponent::OnCharacterStateChanged);
		SwordTraceVFXComponent->DeactivateImmediate();
	}
}

void UCombatComponent::Dispose()
{
	CharacterState->OnStateChanged.RemoveDynamic(this, &UCombatComponent::OnCharacterStateChanged);
}

void UCombatComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputComponent* Input = GetOwner()->GetComponentByClass<UEnhancedInputComponent>())
	{
		Input->BindAction(AttackAction, ETriggerEvent::Started, this, &UCombatComponent::OnAttack);
		Input->BindAction(DodgeAction, ETriggerEvent::Started, this, &UCombatComponent::OnDodge);
		Input->BindAction(TargetLockAction, ETriggerEvent::Started, this, &UCombatComponent::OnTargetLock);
	}
}

void UCombatComponent::OnDodge()
{
	if (DodgeAnim && CharacterState->GetState() != Skill && CharacterState->GetState() != Dragon)
	{
		if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
		{
			if (CharacterMovement->IsFalling() || RPGPlayer->GetCurrentMontage() == DodgeAnim)
			{
				return;
			}
			FVector Direction = RPGPlayer->GetLastMovementInputVector();

			FVector Start = RPGPlayer->GetActorLocation();
			FVector End = Start + Direction * 1000;

			FRotator CurrentRot = GetOwner()->GetActorRotation();
			FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(Start, End);
			FRotator FinalRot = FRotator(CurrentRot.Pitch, TargetRotation.Yaw, TargetRotation.Roll);

			GetOwner()->SetActorRotation(FinalRot);
			CharacterMovement->bOrientRotationToMovement = true;
			RPGPlayer->StopAnimMontage();
			RPGPlayer->PlayAnimMontage(DodgeAnim);
			currentComboIndex = 0;
			bShouldContinueCombo = false;
			CharacterState->ClearState(Attacking);
		}
	}
}

void UCombatComponent::OnTargetLock()
{
	if (!LockTarget)
	{
		if(CharacterState->GetState() == Dragon)
		{
			return;
		}
		if (SoftLockTarget)
		{
			if (IEnemy* Enemy = Cast<IEnemy>(SoftLockTarget))
			{
				Enemy->OnDie.RemoveDynamic(this, &UCombatComponent::OnEnemyDied);
			}
		}
		if (UCameraComponent* Camera = GetOwner()->GetComponentByClass<UCameraComponent>())
		{
			TArray<FHitResult> OutResults;
			FVector Start = Camera->GetComponentLocation();
			FVector End = Start + Camera->GetForwardVector() * 1000;
			TArray<AActor*> ToIgnore;
			ToIgnore.Add(GetOwner());
			TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
			ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

			UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 150,
			                                                 ObjectTypes,
			                                                 false,
			                                                 ToIgnore,
			                                                 EDrawDebugTrace::ForDuration, OutResults, true,
			                                                 FLinearColor::Red,
			                                                 FLinearColor::Green, 1);

			for (FHitResult OutResult : OutResults)
			{
				if (IEnemy* Enemy = Cast<IEnemy>(OutResult.GetActor()))
				{
					LockTarget = OutResult.GetActor();
					Enemy->OnDie.AddDynamic(this, &UCombatComponent::OnEnemyDied);
					return;
				}
			}
		}
	}
	else
	{
		if (IEnemy* Enemy = Cast<IEnemy>(LockTarget))
		{
			Enemy->OnDie.RemoveDynamic(this, &UCombatComponent::OnEnemyDied);
		}
		LockTarget = nullptr;
	}
}

void UCombatComponent::OnEnemyDied()
{
	if (IEnemy* Enemy = Cast<IEnemy>(LockTarget))
	{
		Enemy->OnDie.RemoveDynamic(this, &UCombatComponent::OnEnemyDied);
	}
	LockTarget = nullptr;
	SoftLockOff();
}

void UCombatComponent::SoftLockOn()
{
	bAttackChangeRotation = true;
	attackRotAlpha = 0;
	if (LockTarget || SoftLockTarget) return;

	if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
	{
		if (RPGPlayer->GetState() == Dragon)
		{
			return;
		}
		FVector Direction = RPGPlayer->GetLastMovementInputVector();

		TArray<FHitResult> OutResults;
		FVector Start = RPGPlayer->GetActorLocation();
		FVector End = Start + Direction * 1000;

		TArray<AActor*> ToIgnore;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 100,
		                                                 ObjectTypes,
		                                                 false,
		                                                 ToIgnore,
		                                                 EDrawDebugTrace::None, OutResults, true, FLinearColor::Red,
		                                                 FLinearColor::Green, 1);

		for (FHitResult OutResult : OutResults)
		{
			if (IEnemy* Enemy = Cast<IEnemy>(OutResult.GetActor()))
			{
				SoftLockTarget = OutResult.GetActor();
				Enemy->OnDie.AddDynamic(this, &UCombatComponent::OnEnemyDied);
				return;
			}
		}

		SoftLockTarget = nullptr;
	}
}

bool UCombatComponent::CanDamage()
{
	if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
	{
		return RPGPlayer->GetCurrentMontage() != DodgeAnim;
	}
	return true;
}

void UCombatComponent::OnCharacterStateChanged(ECharacterState State)
{
	if (State != Attacking)
	{
		bShouldContinueCombo = false;
	}
}

void UCombatComponent::SoftLockOff()
{
	if(SoftLockTarget)
	{
		if (IEnemy* Enemy = Cast<IEnemy>(SoftLockTarget))
		{
			Enemy->OnDie.RemoveDynamic(this, &UCombatComponent::OnEnemyDied);
		}
	}
	SoftLockTarget = nullptr;
}

void UCombatComponent::StartSwordTrace()
{
	DamagedActors.Empty();
	CharacterMovement->MaxWalkSpeed = 100.f;
	bIsTracingSword = true;
	bCanSlowTime = true;
	bAttackChangeRotation = false;

	SwordTraceVFXComponent->ActivateSystem(true);
	FVector Start = CharacterMesh->GetSocketLocation("weapon_base");
	FVector End = CharacterMesh->GetSocketLocation("weapon_tip");
	SwordTraceVFXComponent->SetVectorParameter("BeamStart", Start);
	SwordTraceVFXComponent->SetVectorParameter("Normal", End - Start);
}

void UCombatComponent::EndSwordTrace()
{
	bIsTracingSword = false;
	bCanSlowTime = false;
	CharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = true;
	CharacterMovement->MaxWalkSpeed = 500.f;
	CharacterMovement->bOrientRotationToMovement = true;
	ClearDamageModifier();
	SwordTraceVFXComponent->Deactivate();
}

void UCombatComponent::ModifyDamage(float NewDamage)
{
	currentDamage = NewDamage;
}

void UCombatComponent::ClearDamageModifier()
{
	currentDamage = InventoryComponent->GetCurrentWeaponDamage();
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
	timeSLowDuration += DeltaTime;

	if (bTimeSlowed && timeSLowDuration > .02)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
		bTimeSlowed = false;
	}

	if (bIsTracingSword)
	{
		TArray<FHitResult> OutResults;
		FVector Start = CharacterMesh->GetSocketLocation("weapon_base");
		FVector End = CharacterMesh->GetSocketLocation("weapon_tip");
		TArray<AActor*> ToIgnore;
		ToIgnore.Add(GetOwner());
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, SwordTraceRadius,
		                                                 ObjectTypes,
		                                                 false,
		                                                 ToIgnore,
		                                                 EDrawDebugTrace::None, OutResults, true, FLinearColor::Red,
		                                                 FLinearColor::Green, SwordTraceDelay);

		SwordTraceVFXComponent->SetVectorParameter("BeamStart", Start + (End - Start) / 2);
		SwordTraceVFXComponent->SetVectorParameter("Normal", End - Start);

		for (FHitResult OutResult : OutResults)
		{
			if (auto* Damageable = Cast<IIDamageable>(OutResult.GetActor()))
			{
				if (!DamagedActors.Contains(Damageable))
				{
					Damageable->Damage(currentDamage);
					if (auto* Enemy = Cast<IEnemy>(Damageable))
					{
						FVector Location = OutResult.GetActor()->GetActorLocation();
						FVector LaunchDir = Location - GetOwner()->GetActorLocation();

						Enemy->OnHit(GetOwner(), OutResult.Location,
						             LaunchDir * currentDamage);

						if (BloodVFX)
						{
							UNiagaraComponent* BloodFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,
								BloodVFX, OutResult.Location,
								UKismetMathLibrary::FindLookAtRotation(End, OutResult.Location),
								FVector(1.f, 1.f, 1.f), true, true,
								ENCPoolMethod::None, true);
						}

						if (!bTimeSlowed && bCanSlowTime)
						{
							UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 0.2);
							timeSLowDuration = 0;
							bTimeSlowed = true;
							bCanSlowTime = false;
						}
					}

					if (DamageIndicator)
					{
						ADamageIndicatorActor* Damage = GetWorld()->SpawnActor<ADamageIndicatorActor>(
							DamageIndicator, OutResult.Location, FRotator::ZeroRotator);
						if (Damage)
						{
							Damage->Show(currentDamage);
						}
					}

					DamagedActors.Add(Damageable);
				}
			}
		}
	}

	if (bAttackChangeRotation && (SoftLockTarget || LockTarget))
	{
		CharacterMovement->bOrientRotationToMovement = false;
		attackRotAlpha += DeltaTime * 20;
		AActor* Target = LockTarget ? LockTarget : SoftLockTarget;
		FRotator CurrentRot = GetOwner()->GetActorRotation();
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			GetOwner()->GetActorLocation(), Target->GetActorLocation());
		FRotator FinalRot = FRotator(CurrentRot.Pitch, TargetRotation.Yaw, TargetRotation.Roll);

		GetOwner()->SetActorRotation(UKismetMathLibrary::RLerp(CurrentRot, FinalRot, attackRotAlpha, false));
		if (attackRotAlpha >= 1)
		{
			bAttackChangeRotation = false;
		}
	}

	if(SpawnedAirFireBlow && SpawnedAirFireBlow.Get()->IsActive())
	{
		HandleAirFireBlowTrace();
	}
}

void UCombatComponent::OnAttack()
{
	if (CharacterState->GetState() == Nothing)
	{
		if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
		{
			if ((CharacterMovement->IsFalling()))
			{
				return;
			}
			else if (RPGPlayer->GetCurrentMontage() == DodgeAnim)
			{
				if (RPGPlayer->InventoryComponent->HasEquippedWeapon())
				{
					bShouldPerformAfterDodgeAttack = true;
				}
			}
			else if (RPGPlayer->InventoryComponent->HasEquippedWeapon())
			{
				currentComboIndex = 0;
				PlayMontage(NormalAttackComboAnimations[currentComboIndex]);
				CharacterState->SetState(Attacking);
				bShouldContinueCombo = false;
			}
		}
	}
	else if (CharacterState->GetState() == Attacking)
	{
		bShouldContinueCombo = true;
	}
	else if (CharacterState->GetState() == Dragon)
	{
		if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
		{
			if(RPGPlayer->GetCharacterMovement()->MovementMode == MOVE_Flying)
			{
				if(!SpawnedAirFireBlow || !SpawnedAirFireBlow.Get()->IsActive())
				{
					SpawnedAirFireBlow = UNiagaraFunctionLibrary::SpawnSystemAttached(AirFireBlow, RPGPlayer->GetMesh(), FName("FireSocket"),
					FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::SnapToTarget, true);
				}
			}
			else
			{
				auto* Montage = RPGPlayer->GetCurrentMontage();
				if (DragonAttackComboAnimations.Contains(Montage))
				{
					bShouldContinueCombo = true;
				}
				else
				{
					currentComboIndex = 0;
					PlayMontage(DragonAttackComboAnimations[currentComboIndex]);
					bShouldContinueCombo = false;
				}
			}
		}
	}
}

void UCombatComponent::TryContinueCombo()
{
	if (bShouldPerformAfterDodgeAttack)
	{
		currentComboIndex = -1;
		PlayMontage(AfterDodgeAttack);
		CharacterState->SetState(Attacking);
		bShouldContinueCombo = false;
		bShouldPerformAfterDodgeAttack = false;
		return;
	}
	if (CharacterState->GetState() == Attacking)
	{
		if (bShouldContinueCombo)
		{
			if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
			{
				if (RPGPlayer->InventoryComponent->HasEquippedWeapon())
				{
					currentComboIndex++;
					if (NormalAttackComboAnimations.Num() <= currentComboIndex)
					{
						currentComboIndex = 0;
					}
					PlayMontage(NormalAttackComboAnimations[currentComboIndex]);

					bShouldContinueCombo = false;
				}
			}
		}
		else
		{
			CharacterState->ClearState(Attacking);
			SoftLockOff();
		}
	}
	else if (CharacterState->GetState() == Dragon)
	{
		if (bShouldContinueCombo)
		{
			currentComboIndex++;
			if (DragonAttackComboAnimations.Num() <= currentComboIndex)
			{
				currentComboIndex = 0;
			}
			PlayMontage(DragonAttackComboAnimations[currentComboIndex]);

			bShouldContinueCombo = false;
		}
	}
}

void UCombatComponent::PlayMontage(UAnimMontage* Montage)
{
	if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
	{
		SoftLockOn();
		RPGPlayer->PlayAnimMontage(Montage);
	}
}

void UCombatComponent::TryDodgeSpecialAttack()
{
	if (bShouldPerformAfterDodgeAttack)
	{
		TryContinueCombo();
	}
}

AActor* UCombatComponent::GetLockTarget()
{
	return LockTarget;
}

AActor* UCombatComponent::GetSoftLockTarget()
{
	return SoftLockTarget;
}

void UCombatComponent::TryDamageByAbility(const FVector Position, float Damage, const float Range)
{
	TArray<FHitResult> OutResults;
	TArray<AActor*> ToIgnore;
	ToIgnore.Add(GetOwner());
	DamagedActors.Empty();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Position, Position, Range,
	                                                 ObjectTypes,
	                                                 false,
	                                                 ToIgnore,
	                                                 EDrawDebugTrace::None, OutResults, true, FLinearColor::Red,
	                                                 FLinearColor::Green, 2);

	for (FHitResult OutResult : OutResults)
	{
		if (auto* Damageable = Cast<IIDamageable>(OutResult.GetActor()))
		{
			if (!DamagedActors.Contains(Damageable))
			{
				Damageable->Damage(Damage);
				DamagedActors.Add(Damageable);
				if (DamageIndicator)
				{
					// self destroyed
					ADamageIndicatorActor* DamageIndicatorActor = GetWorld()->SpawnActor<ADamageIndicatorActor>(
						DamageIndicator, OutResult.Location, FRotator::ZeroRotator);
					if (DamageIndicatorActor)
					{
						DamageIndicatorActor->Show(Damage);
					}
				}
			}
		}
	}

	DamagedActors.Empty();
}


void UCombatComponent::HandleAirFireBlowTrace()
{
	FVector Start = CharacterMesh->GetSocketLocation("FireSocket");
	FVector End = Start + UKismetMathLibrary::GetForwardVector(CharacterMesh->GetSocketRotation("FireSocket")) * 1000;
	TArray<FHitResult> OutResults;
	TArray<AActor*> ToIgnore;
	ToIgnore.Add(GetOwner());
	DamagedActors.Empty();
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 500,
													 ObjectTypes,
													 false,
													 ToIgnore,
													 EDrawDebugTrace::None, OutResults, true, FLinearColor::Red,
													 FLinearColor::Green, 2);

	for (FHitResult OutResult : OutResults)
	{
		if (auto* Damageable = Cast<IIDamageable>(OutResult.GetActor()))
		{
			if (!DamagedActors.Contains(Damageable))
			{
				Damageable->Damage(AirFireBlowDamage);
				DamagedActors.Add(Damageable);
			}
		}
	}

	DamagedActors.Empty();
}
