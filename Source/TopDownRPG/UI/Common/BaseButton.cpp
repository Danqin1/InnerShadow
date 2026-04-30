// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseButton.h"

#include "Blueprint/WidgetTree.h"

bool UBaseButton::Initialize()
{
	BuildDefaultLayout();
	const bool bInitializedThisCall = Super::Initialize();
	ApplyText();
	return bInitializedThisCall;
}

void UBaseButton::NativePreConstruct()
{
	Super::NativePreConstruct();
	ApplyText();
}

void UBaseButton::SetText(FString newText)
{
	Text = newText;
	BuildDefaultLayout();
	ApplyText();
}

void UBaseButton::BuildDefaultLayout()
{
	if (GetClass() != UBaseButton::StaticClass())
	{
		return;
	}

	if (!WidgetTree)
	{
		WidgetTree = NewObject<UWidgetTree>(this, TEXT("WidgetTree"), RF_Transient);
	}

	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	ButtonText = WidgetTree->ConstructWidget<UCommonTextBlock>(UCommonTextBlock::StaticClass(), TEXT("ButtonText"));
	if (!ButtonText)
	{
		return;
	}

	ButtonText->SetJustification(ETextJustify::Center);
	WidgetTree->RootWidget = ButtonText;
}

void UBaseButton::ApplyText() const
{
	if (ButtonText)
	{
		ButtonText->SetText(FText::FromString(Text));
	}
}
