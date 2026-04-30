#include "RunUpgradeChoiceUI.h"

#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "GameFramework/PlayerController.h"
#include "TopDownRPG/Progression/RunUpgradeSystem.h"
#include "TopDownRPG/UI/Progression/RunUpgradeChoiceCard.h"
#include "Widgets/SWidget.h"

TSharedRef<SWidget> URunUpgradeChoiceUI::RebuildWidget()
{
	BuildDefaultLayout();
	return Super::RebuildWidget();
}

void URunUpgradeChoiceUI::NativeConstruct()
{
	Super::NativeConstruct();

	SetGameplayInputEnabled(false);
	RefreshChoices();
}

void URunUpgradeChoiceUI::NativeDestruct()
{
	ClearChoiceCards();
	SetGameplayInputEnabled(true);

	Super::NativeDestruct();
}

bool URunUpgradeChoiceUI::RefreshChoices()
{
	ClearChoiceCards();
	bSelectionLocked = false;

	if (!ChoicesBox)
	{
		return false;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return false;
	}

	URunUpgradeSystem* RunUpgradeSystem = GameInstance->GetSubsystem<URunUpgradeSystem>();
	if (!RunUpgradeSystem)
	{
		return false;
	}

	const int32 RequestedChoices = FMath::Max(0, ChoiceCount);
	const TArray<FRunUpgradeData> UpgradeChoices = RunUpgradeSystem->GetRandomUpgradeChoices(RequestedChoices);
	if (UpgradeChoices.IsEmpty())
	{
		return false;
	}

	TSubclassOf<URunUpgradeChoiceCard> WidgetClass = CardClass;
	if (!WidgetClass)
	{
		WidgetClass = URunUpgradeChoiceCard::StaticClass();
	}

	for (const FRunUpgradeData& Upgrade : UpgradeChoices)
	{
		URunUpgradeChoiceCard* Card = GetOwningPlayer()
			? CreateWidget<URunUpgradeChoiceCard>(GetOwningPlayer(), WidgetClass)
			: CreateWidget<URunUpgradeChoiceCard>(GetWorld(), WidgetClass);

		if (!Card)
		{
			continue;
		}

		ChoicesBox->AddChild(Card);
		if (UHorizontalBoxSlot* CardSlot = Cast<UHorizontalBoxSlot>(Card->Slot))
		{
			CardSlot->SetPadding(FMargin(8.f, 0.f));
			CardSlot->SetHorizontalAlignment(HAlign_Center);
			CardSlot->SetVerticalAlignment(VAlign_Center);
		}

		Card->SetUpgradeData(Upgrade);
		Card->OnSelected.AddDynamic(this, &URunUpgradeChoiceUI::HandleUpgradeSelected);
		ChoiceCards.Add(Card);
	}

	return ChoiceCards.Num() > 0;
}

void URunUpgradeChoiceUI::HandleUpgradeSelected(FRunUpgradeData Upgrade)
{
	if (bSelectionLocked || Upgrade.ID.IsNone())
	{
		return;
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (!GameInstance)
	{
		return;
	}

	URunUpgradeSystem* RunUpgradeSystem = GameInstance->GetSubsystem<URunUpgradeSystem>();
	if (!RunUpgradeSystem || !RunUpgradeSystem->ApplyUpgradeByID(Upgrade.ID))
	{
		return;
	}

	bSelectionLocked = true;
	for (URunUpgradeChoiceCard* Card : ChoiceCards)
	{
		if (Card)
		{
			Card->SetIsEnabled(false);
		}
	}

	OnUpgradeSelected.Broadcast(Upgrade);

	if (bRemoveFromParentOnSelection)
	{
		RemoveFromParent();
	}
}

void URunUpgradeChoiceUI::BuildDefaultLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
	WidgetTree->RootWidget = Root;

	UVerticalBox* Container = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("UpgradeChoiceContainer"));
	UCanvasPanelSlot* ContainerSlot = Root->AddChildToCanvas(Container);
	ContainerSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	ContainerSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	ContainerSlot->SetPosition(FVector2D::ZeroVector);
	ContainerSlot->SetSize(FVector2D(860.f, 240.f));

	TitleText = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), TEXT("TitleText"));
	TitleText->SetText(FText::FromString(TEXT("Choose Upgrade")));
	TitleText->SetJustification(ETextJustify::Center);
	UVerticalBoxSlot* TitleSlot = Container->AddChildToVerticalBox(TitleText);
	TitleSlot->SetHorizontalAlignment(HAlign_Fill);
	TitleSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 16.f));

	ChoicesBox = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("ChoicesBox"));
	UVerticalBoxSlot* ChoicesSlot = Container->AddChildToVerticalBox(ChoicesBox);
	ChoicesSlot->SetHorizontalAlignment(HAlign_Center);
	ChoicesSlot->SetVerticalAlignment(VAlign_Center);
}

void URunUpgradeChoiceUI::ClearChoiceCards()
{
	for (URunUpgradeChoiceCard* Card : ChoiceCards)
	{
		if (Card)
		{
			Card->OnSelected.RemoveDynamic(this, &URunUpgradeChoiceUI::HandleUpgradeSelected);
		}
	}

	ChoiceCards.Empty();

	if (ChoicesBox)
	{
		ChoicesBox->ClearChildren();
	}
}

void URunUpgradeChoiceUI::SetGameplayInputEnabled(bool bEnabled) const
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

	PlayerController->SetShowMouseCursor(!bEnabled);

	if (bEnabled)
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
	else
	{
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}
}
