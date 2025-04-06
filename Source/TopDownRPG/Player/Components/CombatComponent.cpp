// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"

#include "AbilityComponent.h"
#include "EnhancedInputComponent.h"
#include "MovieSceneTracksComponentTypes.h"
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
	SecondSwordTraceVFXComponent = CreateDefaultSubobject<UNiagaraComponent>("SecondSwordTraceVFXComponent");
}

void UCombatComponent::SetupComponent(UPlayerSettings* Settings)
{
	Super::SetupComponent(Settings);
	
	if (auto* Character = Cast<ARPGCharacter>(GetOwner()))
	{
		CharacterMesh = Character->GetMesh();
		CharacterMovement = Character->GetCharacterMovement();
		InventoryComponent = Character->InventoryComponent;
		ClearDamageModifier();
		CharacterState = Cast<IICharacterState>(Character);
		check(CharacterState.Get());
		SwordTraceVFXComponent->AttachToComponent(CharacterMesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SwordVFX");

		CharacterState->OnStateChanged.AddDynamic(this, &UCombatComponent::OnCharacterStateChanged);
		SwordTraceVFXComponent->DeactivateImmediate();
		SecondSwordTraceVFXComponent->DeactivateImmediate();
	}

	if (UEnhancedInputComponent* Input = GetOwner()->GetComponentByClass<UEnhancedInputComponent>())
	{
		Input->BindAction(PlayerSettings->AttackAction, ETriggerEvent::Started, this, &UCombatComponent::OnAttack);
		Input->BindAction(PlayerSettings->DodgeAction, ETriggerEvent::Started, this, &UCombatComponent::OnDodge);
	}
}

void UCombatComponent::Dispose()
{
	CharacterState->OnStateChanged.RemoveDynamic(this, &UCombatComponent::OnCharacterStateChanged);
}

void UCombatComponent::OnDodge()
{
	if (CharacterState->GetState() == ECharacterState::Darkness)
	{
		return;
	}
	if (PlayerSettings->DodgeAnim)
	{
		if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
		{
			if (CharacterMovement->IsFalling() || RPGPlayer->GetCurrentMontage() == PlayerSettings->DodgeAnim)
			{
				return;
			}

			if(UPlayerStatsComponent* PlayerStats = GetOwner()->GetComponentByClass<UPlayerStatsComponent>())
			{
				PlayerStats->AddDarkness(PlayerSettings->DashDarknessCost);
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
			RPGPlayer->PlayAnimMontage(PlayerSettings->DodgeAnim);
			currentComboIndex = 0;
			bShouldContinueCombo = false;
			CharacterState->ClearState(Attacking);
		}
	}
}

void UCombatComponent::OnEnemyDied()
{
	SoftLockOff();
}

void UCombatComponent::SoftLockOn()
{
	bAttackChangeRotation = true;
	attackRotAlpha = 0;
	if (SoftLockTarget) return;

	if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
	{
		FVector Direction = RPGPlayer->GetLastMovementInputVector();

		TArray<FHitResult> OutResults;
		FVector Start = RPGPlayer->GetActorLocation();
		FVector End = Start + Direction * 1000;

		TArray<AActor*> ToIgnore;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, Start, PlayerSettings->SoftLockDetectionRadius,
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
		return RPGPlayer->GetCurrentMontage() != PlayerSettings->DodgeAnim;
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
	if (!CharacterMovement) return;
	
	CharacterMovement->MaxWalkSpeed = 100.f;
	bIsTracingSword = true;
	//bCanSlowTime = true;
	bAttackChangeRotation = false;

	if (SwordTraceVFXComponent)
	{
		SwordTraceVFXComponent->ActivateSystem(true);
		FVector Start = CharacterMesh->GetSocketLocation("weapon_base");
		FVector End = CharacterMesh->GetSocketLocation("weapon_tip");
		SwordTraceVFXComponent->SetVectorParameter("BeamStart", Start);
		SwordTraceVFXComponent->SetVectorParameter("Normal", End - Start);
		SwordTraceVFXComponent->SetVectorParameter("BeamEnd", End);

		/*Start = CharacterMesh->GetSocketLocation("WeaponL");
		End = CharacterMesh->GetSocketLocation("WeaponLTip");
		SecondSwordTraceVFXComponent->SetVectorParameter("BeamStart", Start);
		SecondSwordTraceVFXComponent->SetVectorParameter("Normal", End - Start);
		SecondSwordTraceVFXComponent->SetVectorParameter("BeamEnd", End);*/
	}
}

void UCombatComponent::EndSwordTrace()
{
	bIsTracingSword = false;
	//bCanSlowTime = false;
	CharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = true;
	CharacterMovement->MaxWalkSpeed = 500.f;
	CharacterMovement->bOrientRotationToMovement = true;
	ClearDamageModifier();
	SwordTraceVFXComponent->Deactivate();
	//SecondSwordTraceVFXComponent->Deactivate();
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
	/*timeSLowDuration += DeltaTime;

	if (bTimeSlowed && timeSLowDuration > .02)
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
		bTimeSlowed = false;
	}*/

	if (bIsTracingSword)
	{
		TArray<FHitResult> OutResults;
		FVector Start = CharacterMesh->GetSocketLocation("weapon_base");
		FVector End = CharacterMesh->GetSocketLocation("weapon_tip");
		TArray<AActor*> ToIgnore;
		ToIgnore.Add(GetOwner());
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, PlayerSettings->SwordTraceRadius,
		                                                 ObjectTypes,
		                                                 false,
		                                                 ToIgnore,
		                                                 EDrawDebugTrace::None, OutResults, true, FLinearColor::Red,
		                                                 FLinearColor::Green, PlayerSettings->SwordTraceDelay);

		SwordTraceVFXComponent->SetVectorParameter("BeamStart", Start + (End - Start) / 2);
		SwordTraceVFXComponent->SetVectorParameter("Normal", End - Start);
		SwordTraceVFXComponent->SetVectorParameter("BeamEnd", End);

		DealSwordDamage(OutResults, End);

		// second sword
		/*Start = CharacterMesh->GetSocketLocation("WeaponL");
		End = CharacterMesh->GetSocketLocation("WeaponLTip");
		TArray<FHitResult> OutResultsSecondSword;
		
		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, PlayerSettings->SwordTraceRadius,
														 ObjectTypes,
														 false,
														 ToIgnore,
														 EDrawDebugTrace::None, OutResultsSecondSword, true, FLinearColor::Red,
														 FLinearColor::Green, PlayerSettings->SwordTraceDelay);

		OutResults.Append(OutResultsSecondSword);

		SecondSwordTraceVFXComponent->SetVectorParameter("BeamStart", Start + (End - Start) / 2);
		SecondSwordTraceVFXComponent->SetVectorParameter("Normal", End - Start);
		SecondSwordTraceVFXComponent->SetVectorParameter("BeamEnd", End);
		
		DamagedActors.Empty();
		DealSwordDamage(OutResults, End);*/
	}

	if (bAttackChangeRotation && SoftLockTarget && PlayerSettings->UseSoftLock)
	{
		CharacterMovement->bOrientRotationToMovement = false;
		attackRotAlpha += DeltaTime * PlayerSettings->SoftLockRotationSpeed;
		AActor* Target = SoftLockTarget;
		FRotator CurrentRot = GetOwner()->GetActorRotation();
		FVector TargetLoc = Target->GetActorLocation();
		TargetLoc.Z = GetOwner()->GetActorLocation().Z;
		FRotator TargetRotation = UKismetMathLibrary::FindLookAtRotation(
			GetOwner()->GetActorLocation(), Target->GetActorLocation());
		FRotator FinalRot = FRotator(CurrentRot.Pitch, TargetRotation.Yaw, TargetRotation.Roll);

		GetOwner()->SetActorRotation(UKismetMathLibrary::RLerp(CurrentRot, FinalRot, attackRotAlpha, false));
		if (attackRotAlpha >= 1)
		{
			bAttackChangeRotation = false;
		}
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
			if (RPGPlayer->GetCurrentMontage() == PlayerSettings->DodgeAnim)
			{
				if (RPGPlayer->InventoryComponent->HasEquippedWeapon())
				{
					bShouldPerformAfterDodgeAttack = true;
				}
			}
			else if (RPGPlayer->InventoryComponent->HasEquippedWeapon())
			{
				currentComboIndex = 0;
				PlayMontage(PlayerSettings->NormalAttackComboAnimations[currentComboIndex]);
				CharacterState->SetState(Attacking);
				bShouldContinueCombo = false;
			}
		}
	}
	else if (CharacterState->GetState() == Attacking)
	{
		bShouldContinueCombo = true;
	}
}

void UCombatComponent::TryContinueCombo()
{
	if (bShouldPerformAfterDodgeAttack)
	{
		currentComboIndex = -1;
		PlayMontage(PlayerSettings->AfterDodgeAttack);
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
					if (PlayerSettings->NormalAttackComboAnimations.Num() <= currentComboIndex)
					{
						currentComboIndex = 0;
					}
					PlayMontage(PlayerSettings->NormalAttackComboAnimations[currentComboIndex]);

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
				if (PlayerSettings->DamageIndicator)
				{
					// self destroyed
					ADamageIndicatorActor* DamageIndicatorActor = GetWorld()->SpawnActor<ADamageIndicatorActor>(
						PlayerSettings->DamageIndicator, OutResult.Location, FRotator::ZeroRotator);
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

void UCombatComponent::DealSwordDamage(TArray<FHitResult> Hitted, FVector WeaponTipEnd)
{
	for (FHitResult OutResult : Hitted)
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
								 LaunchDir * PlayerSettings->PushEnemiesStrength);

					if (PlayerSettings->BloodVFX)
					{
						UNiagaraComponent* BloodFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,
							PlayerSettings->BloodVFX, OutResult.Location,
							UKismetMathLibrary::FindLookAtRotation(WeaponTipEnd, OutResult.Location),
							FVector(1.f, 1.f, 1.f), true, true,
							ENCPoolMethod::None, true);
					}
				}

				if (PlayerSettings->DamageIndicator)
				{
					ADamageIndicatorActor* Damage = GetWorld()->SpawnActor<ADamageIndicatorActor>(
						PlayerSettings->DamageIndicator, OutResult.Location, FRotator::ZeroRotator);
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