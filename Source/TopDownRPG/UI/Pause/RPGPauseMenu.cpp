#include "RPGPauseMenu.h"

#include "Blueprint/WidgetTree.h"
#include "Components/Border.h"
#include "Components/Button.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/WidgetSwitcher.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "TopDownRPG/UI/Pause/RPGPauseActionsWidget.h"
#include "TopDownRPG/UI/Settings/RPGSettingsWidget.h"
#include "TopDownRPG/UI/Stats/RPGStatsWidget.h"
#include "Widgets/SWidget.h"

TSharedRef<SWidget> URPGPauseMenu::RebuildWidget()
{
	BuildDefaultLayout();
	return Super::RebuildWidget();
}

void URPGPauseMenu::NativeConstruct()
{
	Super::NativeConstruct();

	RebuildTabPages();
	BindControls();
	ApplyMenuInput();

	if (bPauseGameOnOpen && GetWorld())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		bAppliedPauseState = true;
	}

	SelectTab(DefaultTab);
	RefreshStats();
}

void URPGPauseMenu::NativeDestruct()
{
	UnbindControls();

	if (bAppliedPauseState && GetWorld())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		bAppliedPauseState = false;
	}

	if (bRestoreGameInputOnClose)
	{
		RestoreGameInput();
	}

	Super::NativeDestruct();
}

void URPGPauseMenu::SelectTab(EPauseMenuTab Tab)
{
	if (TabSwitcher)
	{
		const int32 TabIndex = static_cast<int32>(Tab);
		if (TabSwitcher->GetNumWidgets() > TabIndex)
		{
			TabSwitcher->SetActiveWidgetIndex(TabIndex);
		}
	}

	if (PauseTabButton)
	{
		PauseTabButton->SetIsEnabled(Tab != EPauseMenuTab::Pause);
	}

	if (StatsTabButton)
	{
		StatsTabButton->SetIsEnabled(Tab != EPauseMenuTab::Stats);
	}

	if (SettingsTabButton)
	{
		SettingsTabButton->SetIsEnabled(Tab != EPauseMenuTab::Settings);
	}

	if (Tab == EPauseMenuTab::Stats)
	{
		RefreshStats();
	}

	OnTabChanged.Broadcast(Tab);
}

void URPGPauseMenu::RefreshStats()
{
	if (StatsWidget)
	{
		StatsWidget->RefreshStats();
	}
}

void URPGPauseMenu::CloseMenu()
{
	OnResumeRequested.Broadcast();
	RemoveFromParent();
}

void URPGPauseMenu::HandlePauseTabClicked()
{
	SelectTab(EPauseMenuTab::Pause);
}

void URPGPauseMenu::HandleStatsTabClicked()
{
	SelectTab(EPauseMenuTab::Stats);
}

void URPGPauseMenu::HandleSettingsTabClicked()
{
	SelectTab(EPauseMenuTab::Settings);
}

void URPGPauseMenu::HandleResumeClicked()
{
	CloseMenu();
}

void URPGPauseMenu::HandleQuitClicked()
{
	OnQuitRequested.Broadcast();

	if (GetWorld())
	{
		UGameplayStatics::SetGamePaused(GetWorld(), false);
	}

	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

void URPGPauseMenu::HandleMasterVolumeChanged(float Value)
{
	OnMasterVolumeChanged.Broadcast(Value);
}

void URPGPauseMenu::HandleMusicVolumeChanged(float Value)
{
	OnMusicVolumeChanged.Broadcast(Value);
}

void URPGPauseMenu::HandleSFXVolumeChanged(float Value)
{
	OnSFXVolumeChanged.Broadcast(Value);
}

void URPGPauseMenu::BuildDefaultLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UCanvasPanel* Root = WidgetTree->ConstructWidget<UCanvasPanel>(UCanvasPanel::StaticClass(), TEXT("Root"));
	WidgetTree->RootWidget = Root;

	UBorder* DimBackground = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("DimBackground"));
	DimBackground->SetBrushColor(FLinearColor(0.f, 0.f, 0.f, 0.55f));
	UCanvasPanelSlot* DimSlot = Root->AddChildToCanvas(DimBackground);
	DimSlot->SetAnchors(FAnchors(0.f, 0.f, 1.f, 1.f));
	DimSlot->SetOffsets(FMargin(0.f));

	UBorder* Panel = WidgetTree->ConstructWidget<UBorder>(UBorder::StaticClass(), TEXT("PausePanel"));
	Panel->SetBrushColor(FLinearColor(0.025f, 0.018f, 0.014f, 0.96f));
	Panel->SetPadding(FMargin(28.f));
	UCanvasPanelSlot* PanelSlot = Root->AddChildToCanvas(Panel);
	PanelSlot->SetAnchors(FAnchors(0.5f, 0.5f));
	PanelSlot->SetAlignment(FVector2D(0.5f, 0.5f));
	PanelSlot->SetSize(FVector2D(780.f, 560.f));

	UVerticalBox* Container = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("Container"));
	Panel->AddChild(Container);

	UTextBlock* TitleText = BuildText(TEXT("TitleText"), TEXT("Inner Shadow"), 30);
	TitleText->SetJustification(ETextJustify::Center);
	UVerticalBoxSlot* TitleSlot = Container->AddChildToVerticalBox(TitleText);
	TitleSlot->SetHorizontalAlignment(HAlign_Fill);
	TitleSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 20.f));

	UHorizontalBox* Tabs = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), TEXT("Tabs"));
	UVerticalBoxSlot* TabsSlot = Container->AddChildToVerticalBox(Tabs);
	TabsSlot->SetHorizontalAlignment(HAlign_Fill);
	TabsSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 24.f));

	PauseTabButton = BuildTextButton(TEXT("PauseTabButton"), TEXT("Pause"));
	StatsTabButton = BuildTextButton(TEXT("StatsTabButton"), TEXT("Stats"));
	SettingsTabButton = BuildTextButton(TEXT("SettingsTabButton"), TEXT("Settings"));

	for (UButton* TabButton : { PauseTabButton, StatsTabButton, SettingsTabButton })
	{
		UHorizontalBoxSlot* TabSlot = Tabs->AddChildToHorizontalBox(TabButton);
		TabSlot->SetPadding(FMargin(4.f, 0.f));
		TabSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	}

	TabSwitcher = WidgetTree->ConstructWidget<UWidgetSwitcher>(UWidgetSwitcher::StaticClass(), TEXT("TabSwitcher"));
	UVerticalBoxSlot* SwitcherSlot = Container->AddChildToVerticalBox(TabSwitcher);
	SwitcherSlot->SetHorizontalAlignment(HAlign_Fill);
	SwitcherSlot->SetVerticalAlignment(VAlign_Fill);
	SwitcherSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
}

UButton* URPGPauseMenu::BuildTextButton(FName WidgetName, const FString& Label)
{
	UButton* Button = WidgetTree->ConstructWidget<UButton>(UButton::StaticClass(), WidgetName);
	UTextBlock* ButtonText = BuildText(FName(*(WidgetName.ToString() + TEXT("_Text"))), Label, 18);
	ButtonText->SetJustification(ETextJustify::Center);
	Button->AddChild(ButtonText);
	return Button;
}

UTextBlock* URPGPauseMenu::BuildText(FName WidgetName, const FString& Label, int32 FontSize) const
{
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), WidgetName);
	Text->SetText(FText::FromString(Label));
	Text->SetColorAndOpacity(FSlateColor(FLinearColor(0.82f, 0.72f, 0.52f, 1.f)));

	FSlateFontInfo Font = Text->GetFont();
	Font.Size = FontSize;
	Text->SetFont(Font);

	return Text;
}

void URPGPauseMenu::RebuildTabPages()
{
	if (!TabSwitcher)
	{
		return;
	}

	TabSwitcher->ClearChildren();

	if (!PauseActionsWidget)
	{
		TSubclassOf<URPGPauseActionsWidget> WidgetClass = PauseActionsWidgetClass;
		if (!WidgetClass)
		{
			WidgetClass = URPGPauseActionsWidget::StaticClass();
		}

		PauseActionsWidget = GetOwningPlayer()
			? CreateWidget<URPGPauseActionsWidget>(GetOwningPlayer(), WidgetClass)
			: CreateWidget<URPGPauseActionsWidget>(GetWorld(), WidgetClass);
	}

	if (!StatsWidget)
	{
		TSubclassOf<URPGStatsWidget> WidgetClass = StatsWidgetClass;
		if (!WidgetClass)
		{
			WidgetClass = URPGStatsWidget::StaticClass();
		}

		StatsWidget = GetOwningPlayer()
			? CreateWidget<URPGStatsWidget>(GetOwningPlayer(), WidgetClass)
			: CreateWidget<URPGStatsWidget>(GetWorld(), WidgetClass);
	}

	if (!SettingsWidget)
	{
		TSubclassOf<URPGSettingsWidget> WidgetClass = SettingsWidgetClass;
		if (!WidgetClass)
		{
			WidgetClass = URPGSettingsWidget::StaticClass();
		}

		SettingsWidget = GetOwningPlayer()
			? CreateWidget<URPGSettingsWidget>(GetOwningPlayer(), WidgetClass)
			: CreateWidget<URPGSettingsWidget>(GetWorld(), WidgetClass);
	}

	AddTabPage(PauseActionsWidget);
	AddTabPage(StatsWidget);
	AddTabPage(SettingsWidget);
}

void URPGPauseMenu::AddTabPage(UWidget* PageWidget)
{
	if (!TabSwitcher || !PageWidget)
	{
		return;
	}

	PageWidget->RemoveFromParent();
	TabSwitcher->AddChild(PageWidget);
}

void URPGPauseMenu::BindControls()
{
	if (PauseTabButton) { PauseTabButton->OnClicked.AddUniqueDynamic(this, &URPGPauseMenu::HandlePauseTabClicked); }
	if (StatsTabButton) { StatsTabButton->OnClicked.AddUniqueDynamic(this, &URPGPauseMenu::HandleStatsTabClicked); }
	if (SettingsTabButton) { SettingsTabButton->OnClicked.AddUniqueDynamic(this, &URPGPauseMenu::HandleSettingsTabClicked); }
	if (PauseActionsWidget)
	{
		PauseActionsWidget->OnResumeRequested.AddUniqueDynamic(this, &URPGPauseMenu::HandleResumeClicked);
		PauseActionsWidget->OnQuitRequested.AddUniqueDynamic(this, &URPGPauseMenu::HandleQuitClicked);
	}
	if (SettingsWidget)
	{
		SettingsWidget->OnMasterVolumeChanged.AddUniqueDynamic(this, &URPGPauseMenu::HandleMasterVolumeChanged);
		SettingsWidget->OnMusicVolumeChanged.AddUniqueDynamic(this, &URPGPauseMenu::HandleMusicVolumeChanged);
		SettingsWidget->OnSFXVolumeChanged.AddUniqueDynamic(this, &URPGPauseMenu::HandleSFXVolumeChanged);
	}
}

void URPGPauseMenu::UnbindControls()
{
	if (PauseTabButton) { PauseTabButton->OnClicked.RemoveDynamic(this, &URPGPauseMenu::HandlePauseTabClicked); }
	if (StatsTabButton) { StatsTabButton->OnClicked.RemoveDynamic(this, &URPGPauseMenu::HandleStatsTabClicked); }
	if (SettingsTabButton) { SettingsTabButton->OnClicked.RemoveDynamic(this, &URPGPauseMenu::HandleSettingsTabClicked); }
	if (PauseActionsWidget)
	{
		PauseActionsWidget->OnResumeRequested.RemoveDynamic(this, &URPGPauseMenu::HandleResumeClicked);
		PauseActionsWidget->OnQuitRequested.RemoveDynamic(this, &URPGPauseMenu::HandleQuitClicked);
	}
	if (SettingsWidget)
	{
		SettingsWidget->OnMasterVolumeChanged.RemoveDynamic(this, &URPGPauseMenu::HandleMasterVolumeChanged);
		SettingsWidget->OnMusicVolumeChanged.RemoveDynamic(this, &URPGPauseMenu::HandleMusicVolumeChanged);
		SettingsWidget->OnSFXVolumeChanged.RemoveDynamic(this, &URPGPauseMenu::HandleSFXVolumeChanged);
	}
}

void URPGPauseMenu::ApplyMenuInput()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController)
	{
		return;
	}

	bPreviousShowMouseCursor = PlayerController->bShowMouseCursor;
	PlayerController->SetShowMouseCursor(true);
	PlayerController->SetInputMode(FInputModeGameAndUI());
	bAppliedMenuInput = true;
}

void URPGPauseMenu::RestoreGameInput()
{
	APlayerController* PlayerController = GetOwningPlayer();
	if (!PlayerController || !bAppliedMenuInput)
	{
		return;
	}

	PlayerController->SetShowMouseCursor(bPreviousShowMouseCursor);
	PlayerController->SetInputMode(FInputModeGameOnly());
	bAppliedMenuInput = false;
}
