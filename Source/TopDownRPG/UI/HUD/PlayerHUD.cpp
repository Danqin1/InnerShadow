// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerHUD.h"

void UPlayerHUD::SetHP(float Percent)
{
	if(HealthBar)
	{
		HealthBar->SetPercent(Percent);
	}
}

void UPlayerHUD::SetAction(int slot, FString name, UTexture2D* icon)
{
	switch (slot)
	{
	case 0:
		Action_0->Icon->SetBrushFromTexture(icon, false);
		Action_0->Name->SetText(FText::FromString(name));
		break;
	case 1:
		Action_1->Icon->SetBrushFromTexture(icon, false);
		Action_1->Name->SetText(FText::FromString(name));
		break;
	case 2:
		Action_2->Icon->SetBrushFromTexture(icon, false);
		Action_2->Name->SetText(FText::FromString(name));
		break;
	case 3:
		Action_3->Icon->SetBrushFromTexture(icon, false);
		Action_3->Name->SetText(FText::FromString(name));
		break;
	}
}

void UPlayerHUD::StateChanged(ECharacterState State)
{
	FString enumName = "ECharacterState";
	if(StateDebug)
	{
		StateDebug->SetText(FText::FromString(EnumToString(enumName, State)));
	}
}

UW_ActionSlot* UPlayerHUD::GetUISlot(int index)
{
	switch (index)
	{
	case 0: return Action_0;
	case 1: return Action_1;
	case 2: return Action_2;
	case 3: return Action_3;
		default: return nullptr;
	}
}

void UPlayerHUD::SetEssence(float X)
{
	if (EssenceBar)
	{
		EssenceBar->SetPercent(X);
	}
}

void UPlayerHUD::SetRage(float X)
{
	if (RageBar)
	{
		RageBar->SetPercent(X);
	}
}


