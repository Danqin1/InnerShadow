// Fill out your copyright notice in the Description page of Project Settings.


#include "W_ActionSlot.h"

#include "TopDownRPG/TopDownRPG.h"

void UW_ActionSlot::UpdateRecharge(float progress)
{
	if(CooldownOverlay)
	{
		CooldownOverlay->SetPercent(progress);
	}
}

void UW_ActionSlot::UpdateData(FAbilityData Value)
{
}
