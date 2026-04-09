// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"

#include "GameFramework/Character.h"
#include "TopDownRPG/DevDebug.h"
#include "TopDownRPG/Ability/AbilitySystem.h"
#include "TopDownRPG/Interfaces/PlayerInterface.h"
#include "TopDownRPG/Player/Components/PlayerStatsComponent.h"


// Sets default values
AAbility::AAbility()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AAbility::BeginPlay()
{
	if (UAbilitySystem* AbilitySystem = GetGameInstance()->GetSubsystem<UAbilitySystem>())
	{
		FAbilityData AbilityData = AbilitySystem->GetAbilityData(ID);
		bIsUnlocked = AbilityData.bIsUnlocked || bIsDefault;
		Level = AbilityData.Level;

		AbilitySystem->OnDataUpdated.AddDynamic(this, &AAbility::OnDataUpdated);
	}
	
	Super::BeginPlay();
}

void AAbility::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UAbilitySystem* AbilitySystem = GetGameInstance()->GetSubsystem<UAbilitySystem>())
	{
		AbilitySystem->OnDataUpdated.RemoveDynamic(this, &AAbility::OnDataUpdated);
	}
	Super::EndPlay(EndPlayReason);
}

bool AAbility::CanUseAbility()
{
	if(!CasterCharacter)
	{
		DevDebug::OnScreenLog("Caster is NULL");
		return false;
	}
	if (!bIsUnlocked)
	{
		DevDebug::OnScreenLog("Ability is not unlocked", FColor::Red);
		return false;
	}

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(CasterCharacter))
	{
		if (PlayerInterface->GetState() == Skill)
		{
			return false;
		}
	}
	
	return RechargeTime <= 0;
}

void AAbility::Activate(ACharacter* Caster)
{
	CasterCharacter = Caster;
	RechargeTime = Cooldown;

	if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(CasterCharacter))
	{
		PlayerInterface->GetPlayerStatsComponent()->AddEssence(GetEssenceGenerated());
		PlayerInterface->StopCurrentAnimation();
	}

	for (auto Effect : Effects)
	{
		FTransform CastLocationTransform = FTransform(CastLocation);
		const FTransform Transform = TargetCharacter ? TargetCharacter->GetTransform() : CastLocationTransform;
		AAbilityEffect* AbilityEffect = GetWorld()->SpawnActor<AAbilityEffect>(Effect, Transform);

		if (AbilityEffect)
		{
			AbilityEffect->Activate(Caster);
		}
		else
		{
			if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "Ability effect spawn failed");
		}
	}

	PrimaryActorTick.SetTickFunctionEnable(true);
}

void AAbility::SetUISlot(UW_ActionSlot* Slot)
{
	UISlot = Slot;
	if(UISlot)
	{
		if(Icon)
		{
			UISlot->Icon->SetBrushFromTexture(Icon);
		}
		UISlot->Name->SetText(FText::FromString(Name));
		UISlot->UpdateRecharge(RechargeTime / Cooldown);
	}
	else
	{
		DevDebug::OnScreenLog("Action UI Slot not exists", FColor::Red);
	}
}

void AAbility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if(RechargeTime > 0)
	{
		if (IPlayerInterface* PlayerInterface = Cast<IPlayerInterface>(CasterCharacter))
		{
			if (PlayerInterface->IsInRage())
			{
				RechargeTime = 0;
			}
		}
		
		RechargeTime = FMath::Max(0, RechargeTime - DeltaSeconds);
		if(UISlot)
		{
			UISlot->UpdateRecharge(RechargeTime / Cooldown);
		}
	}
	else
	{
		PrimaryActorTick.SetTickFunctionEnable(false);
	}
}

void AAbility::OnDataUpdated(FAbilityData Value)
{
	if (Value.ID == ID)	
	{
		bIsUnlocked = Value.bIsUnlocked || bIsDefault;
		Level = Value.Level;

		if (UISlot)
		{
			UISlot->UpdateData(Value);
		}
	}
	else
	{
		DevDebug::OnScreenLog("Ability data updated for another ability", FColor::Red);
	}
}

float AAbility::GetEssenceGenerated() const
{
	return EssenceGain >= 0 ? EssenceGain : Cost;
}
