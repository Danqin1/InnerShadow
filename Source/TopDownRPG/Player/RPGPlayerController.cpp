// Copyright Epic Games, Inc. All Rights Reserved.

#include "RPGPlayerController.h"
#include "GameFramework/Pawn.h"
#include "RPGCharacter.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"

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
	SetControlRotation(FRotator(-30, 0, 0));
}

void ARPGPlayerController::Tick(float DeltaSeconds)
{
}

void ARPGPlayerController::SetupInputComponent()
{
	// set up gameplay key bindings
	Super::SetupInputComponent();

	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARPGPlayerController::Jump);
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
	
	if(IPlayerInterface* RPGCharacter = Cast<IPlayerInterface>(GetCharacter()))
	{
		if(RPGCharacter->GetState() == Interaction 
			|| RPGCharacter->GetState() == Skill
			|| RPGCharacter->GetState() == Block
			|| RPGCharacter->GetState() == Frozen
			|| RPGCharacter->GetState() == Dead)
		{
			return;
		}

	}
	
	GetCharacter()->AddMovementInput(ForwardDirection, MovementVector.Y);
	GetCharacter()->AddMovementInput(RightDirection, MovementVector.X);
}

void ARPGPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	
	if(IPlayerInterface* RPGCharacter = Cast<IPlayerInterface>(GetCharacter()))
	{
		if(RPGCharacter->GetState() == Interaction
			|| RPGCharacter->GetState() == Frozen
			|| RPGCharacter->GetState() == Dead)
		{
			return;
		}
	}
	
	AddYawInput(LookAxisVector.X * CameraSpeedYaw);

	LookAxisVector.Y *= -1;
	float pitchMove = (LookAxisVector.Y / 2)  * CameraSpeedPitch;
	if(PitchInput + pitchMove > PitchLimitMin && PitchInput + pitchMove < PitchLimitMax)
	{
		PitchInput = PitchInput + pitchMove;
		AddPitchInput(pitchMove);
	}
}

void ARPGPlayerController::Jump(const FInputActionValue& Value)
{
	if(IPlayerInterface* RPGCharacter = Cast<IPlayerInterface>(GetCharacter()))
	{
		if (RPGCharacter->IsInRage()
			|| RPGCharacter->GetState() == Frozen
			|| RPGCharacter->GetState() == Dead)
		{
			return;
		}

		RPGCharacter->ResetAttack();
	}
	
	GetCharacter()->Jump();
}
