// Fill out your copyright notice in the Description page of Project Settings.


#include "Ability.h"

#include "GameFramework/Character.h"
#include "TopDownRPG/DevDebug.h"
#include "TopDownRPG/Player/Components/PlayerStatsComponent.h"


// Sets default values
AAbility::AAbility()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

bool AAbility::CanUseAbility()
{
	if(!CasterCharacter)
	{
		DevDebug::OnScreenLog("Caster is NULL");
		return false;
	}
	if (UPlayerStatsComponent* Stats = CasterCharacter->FindComponentByClass<UPlayerStatsComponent>())
	{
		return Stats->GetMana() > ManaCost && RechargeTime <= 0;
	}
	return false;
}

void AAbility::Activate(ACharacter* Caster)
{
	CasterCharacter = Caster;
	RechargeTime = Cooldown;
	if (UPlayerStatsComponent* Stats = CasterCharacter->FindComponentByClass<UPlayerStatsComponent>())
	{
		Stats->RemoveMana(ManaCost);
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
