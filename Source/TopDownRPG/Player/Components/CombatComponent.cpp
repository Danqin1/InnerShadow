// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatComponent.h"

#include "AbilityComponent.h"
#include "EnhancedInputComponent.h"
#include "MovieSceneTracksComponentTypes.h"
#include "NiagaraFunctionLibrary.h"
#include "PlayerStatsComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "NiagaraComponent.h"
#include "TopDownRPG/Interfaces/EnemyInterface.h"
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
		CharacterState = Cast<IPlayerInterface>(Character);
		check(CharacterState.Get());
		SwordTraceVFXComponent->AttachToComponent(CharacterMesh,
		                                          FAttachmentTransformRules::SnapToTargetNotIncludingScale, "SwordVFX");

		CharacterState->OnStateChanged.AddDynamic(this, &UCombatComponent::OnCharacterStateChanged);
		SwordTraceVFXComponent->DeactivateImmediate();
		SecondSwordTraceVFXComponent->DeactivateImmediate();
	}

	if (UEnhancedInputComponent* Input = GetOwner()->GetComponentByClass<UEnhancedInputComponent>())
	{
		Input->BindAction(PlayerSettings->AttackAction, ETriggerEvent::Started, this, &UCombatComponent::OnAttack);
		Input->BindAction(PlayerSettings->BlockAction, ETriggerEvent::Started, this, &UCombatComponent::OnBlockStart);
		Input->BindAction(PlayerSettings->BlockAction, ETriggerEvent::Completed, this, &UCombatComponent::OnBlockEnd);
		Input->BindAction(PlayerSettings->DodgeAction, ETriggerEvent::Started, this, &UCombatComponent::OnDodge);
	}
}

void UCombatComponent::Dispose()
{
	CharacterState->OnStateChanged.RemoveDynamic(this, &UCombatComponent::OnCharacterStateChanged);
}

void UCombatComponent::OnDodge()
{
	if (!(CharacterState->GetState() == ECharacterState::Nothing || CharacterState->GetState() == ECharacterState::Attacking))
	{
		return;
	}
	if (PlayerSettings->DodgeAnim)
	{
		if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
		{
			if (CharacterMovement->IsFalling() || RPGPlayer->GetCurrentMontage() == PlayerSettings->DodgeAnim || RPGPlayer->IsInRage())
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
		
			ResetAttack();
			RPGPlayer->StopAnimMontage();
			RPGPlayer->PlayAnimMontage(PlayerSettings->DodgeAnim);
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

	if (ARPGCharacter* RPGPlayer = Cast<ARPGCharacter>(GetOwner()))
	{
		FVector Direction = RPGPlayer->GetLastMovementInputVector();

		TArray<FHitResult> OutResults;
		FVector Start = RPGPlayer->GetActorLocation();
		FVector End = Start + Direction * PlayerSettings->SoftLockDetectionDistance;

		TArray<AActor*> ToIgnore;
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

		UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End,
		                                                 PlayerSettings->SoftLockDetectionRadius,
		                                                 ObjectTypes,
		                                                 false,
		                                                 ToIgnore,
		                                                 EDrawDebugTrace::None, OutResults, true, FLinearColor::Red,
		                                                 FLinearColor::Green, 1);

		for (FHitResult OutResult : OutResults)
		{
			if (IEnemyInterface* Enemy = Cast<IEnemyInterface>(OutResult.GetActor()))
			{
				SoftLockTarget = OutResult.GetActor();
				Enemy->OnDie.AddUniqueDynamic(this, &UCombatComponent::OnEnemyDied);
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
	if (State == Dead)
	{
		ResetAttack();
	}
}

void UCombatComponent::SoftLockOff()
{
	if (SoftLockTarget)
	{
		if (IEnemyInterface* Enemy = Cast<IEnemyInterface>(SoftLockTarget))
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

	if (InventoryComponent)
	{
		currentDamage = InventoryComponent->GetCurrentWeaponDamage();
		if (UPlayerStatsComponent* Stats = GetOwner()->FindComponentByClass<UPlayerStatsComponent>())
		{
			currentDamage *= Stats->GetOutgoingDamageMultiplier();
		}
	}

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
	}
}

void UCombatComponent::EndSwordTrace()
{
	bIsTracingSword = false;
	//bCanSlowTime = false;
	CharacterMovement->bAllowPhysicsRotationDuringAnimRootMotion = true;
	CharacterMovement->MaxWalkSpeed = CharacterState->IsInRage()
		? 500.f * (PlayerSettings ? PlayerSettings->FrenzyMovementControlMultiplier : 0.4f)
		: 500.f;
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
	currentDamage = InventoryComponent ? InventoryComponent->GetCurrentWeaponDamage() : 0;
}

void UCombatComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                     FActorComponentTickFunction* ThisTickFunction)
{
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
	if (CharacterState->IsInRage())
	{
		return;
	}

	if (CharacterState->GetState() == Nothing)
	{
		TrySoftLockDash();
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
		TrySoftLockDash();
	}
}

void UCombatComponent::OnBlockStart()
{
	if (CharacterState->IsInRage())
	{
		return;
	}

	if (CharacterState->GetState() == Nothing || Attacking == CharacterState->GetState())
	{
		CharacterState->SetState(Block);
	}
}

void UCombatComponent::OnBlockEnd()
{
	if (CharacterState->GetState() == Block)
	{
		CharacterState->SetState(Nothing);
	}
}

void UCombatComponent::TrySoftLockDash()
{
	/*if (SoftLockTarget && PlayerSettings->UseSoftLock)
	{
		if (FVector::Dist(GetOwner()->GetActorLocation(), SoftLockTarget->GetActorLocation()) > 450)
		{
			if (PlayerSettings->SoftLockTargetDashVFX)
			{
				auto vfx = UNiagaraFunctionLibrary::SpawnSystemAttached(PlayerSettings->SoftLockTargetDashVFX, GetOwner()->GetRootComponent(), "", 
					FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
				vfx->Activate();
			}
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), .1f);
			
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this, &TimerHandle]
			{
				if (!SoftLockTarget)
				{
					GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
					UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
				}
				else
				{
					if (FVector::Dist(GetOwner()->GetActorLocation(), SoftLockTarget->GetActorLocation()) < 150)
					{
						GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
						UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1.0f);
						return;
					}
					UGameplayStatics::SetGlobalTimeDilation(GetWorld(), FMath::Max(1, UGameplayStatics::GetGlobalTimeDilation(GetWorld()) + -.05f));
					GetOwner()->SetActorLocation(FMath::Lerp(GetOwner()->GetActorLocation(),SoftLockTarget->GetActorLocation(), 0.05f ));
				}
			},0.01, true);
		}
	}*/
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
		float PlayRate = 1.0f;
		if (UPlayerStatsComponent* Stats = GetOwner()->FindComponentByClass<UPlayerStatsComponent>())
		{
			PlayRate = Stats->GetAttackSpeedMultiplier();
		}
		RPGPlayer->PlayAnimMontage(Montage, PlayRate);
	}
}

void UCombatComponent::TryDodgeSpecialAttack()
{
	if (bShouldPerformAfterDodgeAttack)
	{
		TryContinueCombo();
	}
}

void UCombatComponent::ResetAttack()
{
	currentComboIndex = 0;
	bShouldContinueCombo = false;
	bIsTracingSword = false;
	bAttackChangeRotation = false;
	SoftLockOff();
	CharacterState->ClearState(Attacking);
}

bool UCombatComponent::IsPlayingAttackMontage(UAnimMontage* currentMontage)
{
	for (UAnimMontage* NormalAttackComboAnimation : PlayerSettings->NormalAttackComboAnimations)
	{
		if (currentMontage == NormalAttackComboAnimation)
		{
			return true;
		}
	}
	return false;
}

AActor* UCombatComponent::GetSoftLockTarget()
{
	return SoftLockTarget;
}

void UCombatComponent::TryDamageByAbility(const FVector Position, float Damage, const float Range)
{
	if (UPlayerStatsComponent* Stats = GetOwner()->FindComponentByClass<UPlayerStatsComponent>())
	{
		Damage *= Stats->GetOutgoingDamageMultiplier();
	}

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
		if (auto* Combat = Cast<ICombatInterface>(OutResult.GetActor()))
		{
			if (!DamagedActors.Contains(Combat))
			{
				Combat->Hit(GetOwner(), OutResult.Location,
				            FVector::One(), Damage, true);
				DamagedActors.Add(Combat);
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
	bool canPlayAudio = true;
	if (UPlayerStatsComponent* Stats = GetOwner()->FindComponentByClass<UPlayerStatsComponent>())
	{
		for (FHitResult OutResult : Hitted)
		{
			if (auto* Combat = Cast<ICombatInterface>(OutResult.GetActor()))
			{
				if (!DamagedActors.Contains(Combat))
				{
					FVector Location = OutResult.GetActor()->GetActorLocation();
					FVector LaunchDir = Location - GetOwner()->GetActorLocation();

					Combat->Hit(GetOwner(), OutResult.Location,
					            LaunchDir * PlayerSettings->PushEnemiesStrength, currentDamage, false);

					if (PlayerSettings->BloodVFX)
					{
						UNiagaraComponent* BloodFX = UNiagaraFunctionLibrary::SpawnSystemAtLocation(this,
							PlayerSettings->BloodVFX, OutResult.Location,
							UKismetMathLibrary::FindLookAtRotation(WeaponTipEnd, OutResult.Location),
							FVector(1.f, 1.f, 1.f), true, true,
							ENCPoolMethod::None, true);
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
					DamagedActors.Add(Combat);
					if (canPlayAudio && UGameplayStatics::GetTimeSeconds(GetWorld()) - lastHitAudioPlayedTime >
						PlayerSettings->HitAudioMinDelay)
					{
						UGameplayStatics::GetPlayerCameraManager(GetWorld(), 0)->StartCameraShake(
							PlayerSettings->SwordHitCameraShake);
						UGameplayStatics::PlaySound2D(GetWorld(), PlayerSettings->OnHitSound);
						canPlayAudio = false;
						lastHitAudioPlayedTime = UGameplayStatics::GetTimeSeconds(GetWorld());
					}
				}
			}
		}
	}
}
