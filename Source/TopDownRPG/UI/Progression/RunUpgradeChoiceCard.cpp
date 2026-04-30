#include "RunUpgradeChoiceCard.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Button.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Widgets/SWidget.h"

TSharedRef<SWidget> URunUpgradeChoiceCard::RebuildWidget()
{
	BuildDefaultLayout();
	return Super::RebuildWidget();
}

void URunUpgradeChoiceCard::NativeConstruct()
{
	Super::NativeConstruct();

	if (SelectButton)
	{
		SelectButton->OnClicked.AddUniqueDynamic(this, &URunUpgradeChoiceCard::HandleClicked);
	}

	RefreshDisplay();
}

void URunUpgradeChoiceCard::NativeDestruct()
{
	if (SelectButton)
	{
		SelectButton->OnClicked.RemoveDynamic(this, &URunUpgradeChoiceCard::HandleClicked);
	}

	Super::NativeDestruct();
}

void URunUpgradeChoiceCard::SetUpgradeData(FRunUpgradeData InUpgradeData)
{
	UpgradeData = InUpgradeData;
	RefreshDisplay();
}

FRunUpgradeData URunUpgradeChoiceCard::GetUpgradeData() const
{
	return UpgradeData;
}

void URunUpgradeChoiceCard::HandleClicked()
{
	if (!UpgradeData.ID.IsNone())
	{
		OnSelected.Broadcast(UpgradeData);
	}
}

void URunUpgradeChoiceCard::BuildDefaultLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	USizeBox* Root = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass(), TEXT("Root"));
	Root->SetWidthOverride(260.f);
	Root->SetHeightOverride(180.f);
	WidgetTree->RootWidget = Root;

	SelectButton = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), TEXT("SelectButton"));
	Root->AddChild(SelectButton);

	UVerticalBox* Content = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Content"));
	SelectButton->AddChild(Content);

	NameText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("NameText"));
	NameText->SetJustification(ETextJustify::Center);
	UVerticalBoxSlot* NameSlot = Content->AddChildToVerticalBox(NameText);
	NameSlot->SetHorizontalAlignment(HAlign_Fill);
	NameSlot->SetPadding(FMargin(12.f, 12.f, 12.f, 8.f));

	DescriptionText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("DescriptionText"));
	DescriptionText->SetAutoWrapText(true);
	UVerticalBoxSlot* DescriptionSlot = Content->AddChildToVerticalBox(DescriptionText);
	DescriptionSlot->SetHorizontalAlignment(HAlign_Fill);
	DescriptionSlot->SetPadding(FMargin(12.f, 0.f, 12.f, 12.f));
}

void URunUpgradeChoiceCard::RefreshDisplay()
{
	if (NameText)
	{
		NameText->SetText(FText::FromString(UpgradeData.Name));
	}

	if (DescriptionText)
	{
		DescriptionText->SetText(FText::FromString(UpgradeData.Description));
	}
}
