// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilityComponent.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/DevDebug.h"
#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/Player/RPGPlayerController.h"


// Sets default values for this component's properties
UAbilityComponent::UAbilityComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	PrimaryComponentTick.bStartWithTickEnabled = true;

	CurrentAbilities.SetNum(MAX_ABILITIES_COUNT);
}

void UAbilityComponent::SetupComponent(UPlayerSettings* Settings)
{
	Super::SetupComponent(Settings);
	
	if(ARPGPlayerController* CharacterPC = Cast<ARPGPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController = CharacterPC;
	}

	if(ARPGCharacter* Player = Cast<ARPGCharacter>(GetOwner()))
	{
		PlayerHUD = Player->PlayerHUD;
		CharacterState = Cast<IPlayerInterface>(Player);
		check(CharacterState.Get());
		check(PlayerHUD);
	}
	
	//Initial setup abiities for tests
	ChangeAbilityOnIndex(0, PlayerSettings->Ability1);
	ChangeAbilityOnIndex(1, PlayerSettings->Ability2);
	ChangeAbilityOnIndex(2, PlayerSettings->Ability3);
	ChangeAbilityOnIndex(3, PlayerSettings->Ability4);

	if(UEnhancedInputComponent* Input = GetOwner()->GetComponentByClass<UEnhancedInputComponent>())
	{
		Input->BindAction(PlayerSettings->Ability1Action, ETriggerEvent::Started, this, &UAbilityComponent::OnAbility1);
		Input->BindAction(PlayerSettings->Ability2Action, ETriggerEvent::Started, this, &UAbilityComponent::OnAbility2);
		Input->BindAction(PlayerSettings->Ability3Action, ETriggerEvent::Started, this, &UAbilityComponent::OnAbility3);
		Input->BindAction(PlayerSettings->Ability4Action, ETriggerEvent::Started, this, &UAbilityComponent::OnAbility4);
	}
}

void UAbilityComponent::Dispose()
{}

void UAbilityComponent::TickComponent(float DeltaTime, ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{}

void UAbilityComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

void UAbilityComponent::OnAbility1()
{
	TryUseAbility(CurrentAbilities[0]);
}

void UAbilityComponent::OnAbility2()
{
	TryUseAbility(CurrentAbilities[1]);
}

void UAbilityComponent::OnAbility3()
{
	TryUseAbility(CurrentAbilities[2]);
}

void UAbilityComponent::OnAbility4()
{
	TryUseAbility(CurrentAbilities[3]);
}

void UAbilityComponent::TryUseAbility(AAbility* Ability)
{
	if(Ability && Ability->CanUseAbility())
	{
		CastAbility(Ability);
		CharacterState->SetState(ECharacterState::Skill);
	}
	else
	{
		DevDebug::OnScreenLog("Cant use this ability", FColor::Red);
	}
}

void UAbilityComponent::CastAbility(AAbility* Ability)
{
	if(PlayerController)
	{
		Ability->CastLocation = GetOwner()->GetActorLocation();
		Ability->SetActorLocation(GetOwner()->GetActorLocation());
		Ability->Activate(PlayerController->GetCharacter());
	}
}

void UAbilityComponent::ChangeAbilityOnIndex(int Index, TSubclassOf<AAbility> AbilityClass)
{
	if(Index <  MAX_ABILITIES_COUNT)
	{
		if(CurrentAbilities[Index])
		{
			GetWorld()->DestroyActor(CurrentAbilities[Index]);
		}

		if(AbilityClass)
		{
			CurrentAbilities[Index] = GetWorld()->SpawnActor<AAbility>(AbilityClass);
			if(CurrentAbilities[Index] && PlayerHUD)
			{
				CurrentAbilities[Index]->SetUISlot(PlayerHUD->GetUISlot(Index));
				
				if(ACharacter* Character = Cast<ACharacter>(GetOwner()))
				{
					CurrentAbilities[Index]->CasterCharacter = Character;
				}
				else
				{
					DevDebug::OnScreenLog("Cant set up Caster");
				}
			}
			else
			{
				DevDebug::OnScreenLog("Cant set UI slot", FColor::Red, 5);
			}
		}
		else
		{
			DevDebug::OnScreenLog("Ability not set up: " + FString::SanitizeFloat(Index));
		}
	}
	else
	{
		DevDebug::OnScreenLog("Index higher than abilities array");
	}
}

