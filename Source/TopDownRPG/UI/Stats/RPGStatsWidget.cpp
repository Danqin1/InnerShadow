#include "RPGStatsWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/Player/Components/PlayerStatsComponent.h"
#include "Widgets/SWidget.h"

TSharedRef<SWidget> URPGStatsWidget::RebuildWidget()
{
	BuildDefaultLayout();
	return Super::RebuildWidget();
}

void URPGStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	RefreshStats();
}

void URPGStatsWidget::SetStatsComponent(UPlayerStatsComponent* InStatsComponent)
{
	StatsComponentOverride = InStatsComponent;
	RefreshStats();
}

void URPGStatsWidget::RefreshStats()
{
	UPlayerStatsComponent* StatsComponent = FindPlayerStatsComponent();
	if (!StatsComponent)
	{
		const FText EmptyValue = FText::FromString(TEXT("--"));
		if (HealthValueText) { HealthValueText->SetText(EmptyValue); }
		if (EssenceValueText) { EssenceValueText->SetText(EmptyValue); }
		if (EssenceStateValueText) { EssenceStateValueText->SetText(EmptyValue); }
		if (DamageValueText) { DamageValueText->SetText(EmptyValue); }
		if (IncomingDamageValueText) { IncomingDamageValueText->SetText(EmptyValue); }
		if (AttackSpeedValueText) { AttackSpeedValueText->SetText(EmptyValue); }
		if (HealthRegenValueText) { HealthRegenValueText->SetText(EmptyValue); }
		if (EssenceGainValueText) { EssenceGainValueText->SetText(EmptyValue); }
		return;
	}

	if (HealthValueText)
	{
		HealthValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), StatsComponent->GetHP(), StatsComponent->GetMaxHP())));
	}

	if (EssenceValueText)
	{
		EssenceValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f / %.0f"), StatsComponent->GetEssence(), StatsComponent->GetMaxEssence())));
	}

	if (EssenceStateValueText)
	{
		EssenceStateValueText->SetText(FText::FromString(GetEssenceStateText(StatsComponent)));
	}

	if (DamageValueText)
	{
		DamageValueText->SetText(FText::FromString(FormatPercentMultiplier(StatsComponent->GetOutgoingDamageMultiplier())));
	}

	if (IncomingDamageValueText)
	{
		IncomingDamageValueText->SetText(FText::FromString(FormatPercentMultiplier(StatsComponent->GetIncomingDamageMultiplier())));
	}

	if (AttackSpeedValueText)
	{
		AttackSpeedValueText->SetText(FText::FromString(FormatPercentMultiplier(StatsComponent->GetAttackSpeedMultiplier())));
	}

	if (HealthRegenValueText)
	{
		HealthRegenValueText->SetText(FText::FromString(FString::Printf(TEXT("%.1f / sec"), StatsComponent->GetHealthRegenPerSecond())));
	}

	if (EssenceGainValueText)
	{
		EssenceGainValueText->SetText(FText::FromString(FormatPercentMultiplier(StatsComponent->GetEssenceGainMultiplier())));
	}
}

void URPGStatsWidget::BuildDefaultLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("StatsRoot"));
	WidgetTree->RootWidget = Root;

	AddStatRow(Root, TEXT("Health"), HealthValueText);
	AddStatRow(Root, TEXT("Dark Essence"), EssenceValueText);
	AddStatRow(Root, TEXT("Essence State"), EssenceStateValueText);
	AddStatRow(Root, TEXT("Damage"), DamageValueText);
	AddStatRow(Root, TEXT("Damage Taken"), IncomingDamageValueText);
	AddStatRow(Root, TEXT("Attack Speed"), AttackSpeedValueText);
	AddStatRow(Root, TEXT("HP Regen"), HealthRegenValueText);
	AddStatRow(Root, TEXT("Essence Gain"), EssenceGainValueText);
}

UTextBlock* URPGStatsWidget::BuildText(FName WidgetName, const FString& Label, int32 FontSize) const
{
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), WidgetName);
	Text->SetText(FText::FromString(Label));
	Text->SetColorAndOpacity(FSlateColor(FLinearColor(0.82f, 0.72f, 0.52f, 1.f)));

	FSlateFontInfo Font = Text->GetFont();
	Font.Size = FontSize;
	Text->SetFont(Font);

	return Text;
}

void URPGStatsWidget::AddStatRow(UVerticalBox* Parent, const FString& Label, UTextBlock*& ValueText)
{
	FString SafeLabel = Label;
	SafeLabel.ReplaceInline(TEXT(" "), TEXT(""));

	UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName(*(SafeLabel + TEXT("Row"))));
	UVerticalBoxSlot* RowSlot = Parent->AddChildToVerticalBox(Row);
	RowSlot->SetHorizontalAlignment(HAlign_Fill);
	RowSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 10.f));

	UTextBlock* LabelText = BuildText(FName(*(SafeLabel + TEXT("Label"))), Label, 18);
	UHorizontalBoxSlot* LabelSlot = Row->AddChildToHorizontalBox(LabelText);
	LabelSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

	ValueText = BuildText(FName(*(SafeLabel + TEXT("Value"))), TEXT("--"), 18);
	ValueText->SetJustification(ETextJustify::Right);
	UHorizontalBoxSlot* ValueSlot = Row->AddChildToHorizontalBox(ValueText);
	ValueSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
}

UPlayerStatsComponent* URPGStatsWidget::FindPlayerStatsComponent() const
{
	if (StatsComponentOverride)
	{
		return StatsComponentOverride;
	}

	if (APawn* OwningPawn = GetOwningPlayerPawn())
	{
		if (UPlayerStatsComponent* StatsComponent = OwningPawn->FindComponentByClass<UPlayerStatsComponent>())
		{
			return StatsComponent;
		}
	}

	if (APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0))
	{
		return PlayerPawn->FindComponentByClass<UPlayerStatsComponent>();
	}

	return nullptr;
}

FString URPGStatsWidget::FormatPercentMultiplier(float Multiplier) const
{
	return FString::Printf(TEXT("%.0f%%"), Multiplier * 100.f);
}

FString URPGStatsWidget::GetEssenceStateText(UPlayerStatsComponent* StatsComponent) const
{
	if (!StatsComponent)
	{
		return TEXT("--");
	}

	switch (StatsComponent->GetEssenceState())
	{
	case EDarkEssenceState::Empowered:
		return TEXT("Empowered");
	case EDarkEssenceState::Unstable:
		return TEXT("Unstable");
	case EDarkEssenceState::Frenzy:
		return TEXT("Frenzy");
	case EDarkEssenceState::Normal:
	default:
		return TEXT("Normal");
	}
}
