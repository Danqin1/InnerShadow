// Fill out your copyright notice in the Description page of Project Settings.


#include "InteractionComponent.h"

#include "EnhancedInputComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownRPG/DevDebug.h"
#include "TopDownRPG/Player/RPGCharacter.h"

UInteractionComponent::UInteractionComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = false;
}

void UInteractionComponent::SetupComponent(UPlayerSettings* Settings)
{
	Super::SetupComponent(Settings);
	if(UEnhancedInputComponent* Input = GetOwner()->GetComponentByClass<UEnhancedInputComponent>())
	{
		Input->BindAction(InteractAction, ETriggerEvent::Started, this, &UInteractionComponent::OnInteract);
	}

	if(auto* Character = Cast<ARPGCharacter>(GetOwner()))
	{
		PlayerCamera = Character->GetFollowCamera();
		Character->OnStateChanged.AddDynamic(this, &UInteractionComponent::OnStateChanged);
	}
}

void UInteractionComponent::Dispose()
{
	if(auto* Character = Cast<ARPGCharacter>(GetOwner()))
	{
		Character->OnStateChanged.RemoveDynamic(this, &UInteractionComponent::OnStateChanged);
	}
}

void UInteractionComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UInteractionComponent::SetInteractionTarget(IInteractableInterface* Interactable)
{
	if(Interactable)
	{
		if(InteractionTarget.Get())
		{
			InteractionTarget->SetAsTarget(false);
		}
		InteractionTarget = Interactable;
		InteractionTarget->SetAsTarget(true);
	}
	else
	{
		if(InteractionTarget.Get())
		{
			InteractionTarget->SetAsTarget(false);
		}
		InteractionTarget = nullptr;
	}
}

void UInteractionComponent::OnInteract()
{
	if(CurrentState != Nothing) return;
	
	if(InteractionTarget.Get())
	{
		if(auto * Character = Cast<ARPGCharacter>(GetOwner()))
		{
			InteractionTarget.Get()->Interact(Character);
			SetInteractionTarget(nullptr);
		}
	}
	else
	{
		DevDebug::OnScreenLog("Interaction Target invalid");
	}
}

void UInteractionComponent::OnStateChanged(ECharacterState State)
{
	CurrentState = State;
	if(CurrentState == Nothing)
	{
		SetComponentTickEnabled(true);
	}
	else
	{
		SetComponentTickEnabled(false);
		SetInteractionTarget(nullptr);
	}
}

void UInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	if(PlayerCamera && CurrentState == Nothing)
	{
		FVector Start = PlayerCamera->GetComponentLocation();
		FVector End = Start + PlayerCamera->GetForwardVector() * PlayerSettings->InteractionMaxDistance;
		TArray<AActor*> ToIgnore;
		TArray<FHitResult> OutResults;
		ToIgnore.Add(GetOwner());
		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));
		ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));

		bool hitted = UKismetSystemLibrary::SphereTraceMultiForObjects(GetWorld(), Start, End, 60,
											   ObjectTypes,
											   false,
											   ToIgnore,
											   EDrawDebugTrace::None, OutResults, true, FLinearColor::Red,
											   FLinearColor::Green, DeltaTime * 2);

		if(hitted)
		{
			for (FHitResult OutResult : OutResults)
			{
				if(auto* Interactable = Cast<IInteractableInterface>(OutResult.GetActor()))
				{
					if(FVector::Distance(GetOwner()->GetActorLocation(),OutResult.GetActor()->GetActorLocation()) < Interactable->GetInteractionDistance())
					{
						SetInteractionTarget(Interactable);
						return;
					}
				}
			}
		}
		SetInteractionTarget(nullptr);
	}
	else
	{
		SetInteractionTarget(nullptr);
	}
}

