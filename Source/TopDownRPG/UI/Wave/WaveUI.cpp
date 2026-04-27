// Fill out your copyright notice in the Description page of Project Settings.


#include "WaveUI.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"

TSharedRef<SWidget> UWaveUI::RebuildWidget()
{
	BuildDefaultLayout();
	return Super::RebuildWidget();
}

void UWaveUI::NativeConstruct()
{
	Super::NativeConstruct();

	SetWavesLeft(0);
	SetNextWaveTimerVisible(false);
}

void UWaveUI::BuildDefaultLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
	WidgetTree->RootWidget = Root;

	UVerticalBox* Container = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("WaveContainer"));
	UCanvasPanelSlot* ContainerSlot = Root->AddChildToCanvas(Container);
	ContainerSlot->SetAnchors(FAnchors(0.5f, 0.f));
	ContainerSlot->SetAlignment(FVector2D(0.5f, 0.f));
	ContainerSlot->SetPosition(FVector2D(0.f, 64.f));
	ContainerSlot->SetSize(FVector2D(320.f, 72.f));

	WavesLeftText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("WavesLeftText"));
	WavesLeftText->SetJustification(ETextJustify::Center);
	UVerticalBoxSlot* WavesLeftSlot = Container->AddChildToVerticalBox(WavesLeftText);
	WavesLeftSlot->SetHorizontalAlignment(HAlign_Fill);
	WavesLeftSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 4.f));

	NextWaveTimerText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("NextWaveTimerText"));
	NextWaveTimerText->SetJustification(ETextJustify::Center);
	UVerticalBoxSlot* TimerTextSlot = Container->AddChildToVerticalBox(NextWaveTimerText);
	TimerTextSlot->SetHorizontalAlignment(HAlign_Fill);
	TimerTextSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 3.f));

	USizeBox* TimerBarSizeBox = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("TimerBarSizeBox"));
	TimerBarSizeBox->SetHeightOverride(8.f);
	TimerBarSizeBox->SetWidthOverride(260.f);

	NextWaveTimerBar = WidgetTree->ConstructWidget<UProgressBar>(UProgressBar::StaticClass(), TEXT("NextWaveTimerBar"));
	NextWaveTimerBar->SetPercent(1.f);
	TimerBarSizeBox->AddChild(NextWaveTimerBar);

	UVerticalBoxSlot* TimerBarSlot = Container->AddChildToVerticalBox(TimerBarSizeBox);
	TimerBarSlot->SetHorizontalAlignment(HAlign_Center);
}

void UWaveUI::SetWavesLeft(int32 WavesLeft)
{
	if (WavesLeftText)
	{
		WavesLeftText->SetText(FText::FromString(FString::Printf(TEXT("Waves left: %d"), FMath::Max(0, WavesLeft))));
	}
}

void UWaveUI::SetNextWaveTimer(float TimeRemaining, float TotalDuration)
{
	const float Percent = TotalDuration > KINDA_SMALL_NUMBER
		? FMath::Clamp(TimeRemaining / TotalDuration, 0.f, 1.f)
		: 0.f;

	if (NextWaveTimerBar)
	{
		NextWaveTimerBar->SetPercent(Percent);
	}

	if (NextWaveTimerText)
	{
		const int32 SecondsLeft = FMath::Max(0, FMath::CeilToInt(TimeRemaining));
		NextWaveTimerText->SetText(FText::FromString(FString::Printf(TEXT("Next wave: %ds"), SecondsLeft)));
	}
}

void UWaveUI::SetNextWaveTimerVisible(bool bVisible)
{
	const ESlateVisibility TimerVisibility = bVisible ? ESlateVisibility::HitTestInvisible : ESlateVisibility::Collapsed;

	if (NextWaveTimerText)
	{
		NextWaveTimerText->SetVisibility(TimerVisibility);
	}

	if (NextWaveTimerBar)
	{
		NextWaveTimerBar->SetVisibility(TimerVisibility);
	}
}
