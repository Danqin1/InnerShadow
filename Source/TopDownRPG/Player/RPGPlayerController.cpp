// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGPlayerController.h"
#include "GameFramework/Pawn.h"
#include "RPGCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "TopDownRPG/Enemy/EnemyCharacter.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ARPGPlayerController::ARPGPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bShowMouseCursor = true;
	DefaultMouseCursor = EMouseCursor::Default;
}

void ARPGPlayerController::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
	bEnableMouseOverEvents = true;
	bEnableClickEvents = true;
}

void ARPGPlayerController::Tick(float DeltaSeconds)
{
	if(ARPGCharacter* TopDownCharacter = Cast<ARPGCharacter>(GetCharacter()))
	{
		if(TopDownCharacter->CombatComponent)
		{
			AActor* LockTarget = TopDownCharacter->CombatComponent->GetLockTarget();
			
			if(LockTarget)
			{
				FVector LockTargetPos = LockTarget->GetActorLocation();
				LockTargetPos.Z -= 50;
				SetControlRotation(UKismetMathLibrary::FindLookAtRotation(TopDownCharacter->GetActorLocation(), LockTargetPos));
			}
		}
	}
}

void ARPGPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(ZoomInOutAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::OnZoomInOut);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ARPGPlayerController::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ARPGPlayerController::StopJumping);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Move);
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Completed, this, &ARPGPlayerController::MoveEnd);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Look);
		EnhancedInputComponent->BindAction(DragonForm, ETriggerEvent::Started, this, &ARPGPlayerController::OnDragonForm);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void ARPGPlayerController::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
	if(ARPGCharacter* RPGCharacter = Cast<ARPGCharacter>(GetCharacter()))
	{
		if(RPGCharacter->GetState() == Interaction)
		{
			return;
		}
		if(RPGCharacter->GetState() == Dragon)
		{
			MovementVector.X = 0;
			MovementVector.Y = FMath::Max(0, MovementVector.Y);
		}
	}
	
	GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
}

void ARPGPlayerController::Look(const FInputActionValue& Value)
{
	/*FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if(ARPGCharacter* RPGCharacter = Cast<ARPGCharacter>(GetCharacter()))
	{
		if(RPGCharacter->GetState() == Interaction)
		{
			return;
		}
	}
	
	AddYawInput(LookAxisVector.X);
	AddPitchInput(LookAxisVector.Y / 2);*/
}

void ARPGPlayerController::Jump(const FInputActionValue& Value)
{
	if(auto * RPGCharacter = Cast<ARPGCharacter>(GetCharacter()))
	{
		if(RPGCharacter->GetState() == Skill || RPGCharacter->GetState() == Interaction)
		{
			return;
		}
		if(RPGCharacter->GetState() == Dragon)
		{
			RPGCharacter->ToggleFlying();
			return;
		}
	}
	GetCharacter()->Jump();
}

void ARPGPlayerController::StopJumping(const FInputActionValue& Value)
{
	GetCharacter()->StopJumping();
}

void ARPGPlayerController::OnDragonForm()
{
	/*if(auto* RPGPlayer = Cast<ARPGCharacter>(GetCharacter()))
	{
		RPGPlayer->ChangeForm();
	}*/
}

void ARPGPlayerController::MoveEnd()
{
}

void ARPGPlayerController::OnZoomInOut(const FInputActionValue& Value)
{
	/*if(ARPGCharacter* TopDownCharacter = Cast<ARPGCharacter>(GetCharacter()))
	{
		const float CurrentZoom = TopDownCharacter->GetCameraBoom()->TargetArmLength;
		TopDownCharacter->GetCameraBoom()->TargetArmLength = FMath::Clamp(CurrentZoom + Value.Get<float>() * Settings->ZoomSpeed, Settings->MinCameraDistance, Settings->MaxCameraDistance);
	}*/
}
