// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseButton.h"


void UBaseButton::SetText(FString newText)
{
	Text = newText;
	ButtonText->SetText(FText::FromString(Text));
}
