#include "RPGPauseActionsWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "Widgets/SWidget.h"

TSharedRef<SWidget> URPGPauseActionsWidget::RebuildWidget()
{
	BuildDefaultLayout();
	return Super::RebuildWidget();
}

void URPGPauseActionsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ResumeButton)
	{
		ResumeButton->OnClicked().RemoveAll(this);
		ResumeButton->OnClicked().AddUObject(this, &URPGPauseActionsWidget::HandleResumeClicked);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked().RemoveAll(this);
		QuitButton->OnClicked().AddUObject(this, &URPGPauseActionsWidget::HandleQuitClicked);
	}
}

void URPGPauseActionsWidget::NativeDestruct()
{
	if (ResumeButton)
	{
		ResumeButton->OnClicked().RemoveAll(this);
	}

	if (QuitButton)
	{
		QuitButton->OnClicked().RemoveAll(this);
	}

	Super::NativeDestruct();
}

void URPGPauseActionsWidget::HandleResumeClicked()
{
	OnResumeRequested.Broadcast();
}

void URPGPauseActionsWidget::HandleQuitClicked()
{
	OnQuitRequested.Broadcast();
}

void URPGPauseActionsWidget::BuildDefaultLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("PauseActionsRoot"));
	WidgetTree->RootWidget = Root;

	UTextBlock* PauseTitle = BuildText(TEXT("PauseTitle"), TEXT("Run Paused"), 24);
	PauseTitle->SetJustification(ETextJustify::Center);
	UVerticalBoxSlot* PauseTitleSlot = Root->AddChildToVerticalBox(PauseTitle);
	PauseTitleSlot->SetHorizontalAlignment(HAlign_Fill);
	PauseTitleSlot->SetPadding(FMargin(0.f, 32.f, 0.f, 28.f));

	ResumeButton = BuildTextButton(TEXT("ResumeButton"), TEXT("Resume"));
	UVerticalBoxSlot* ResumeSlot = Root->AddChildToVerticalBox(ResumeButton);
	ResumeSlot->SetHorizontalAlignment(HAlign_Center);
	ResumeSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 12.f));

	QuitButton = BuildTextButton(TEXT("QuitButton"), TEXT("Quit"));
	UVerticalBoxSlot* QuitSlot = Root->AddChildToVerticalBox(QuitButton);
	QuitSlot->SetHorizontalAlignment(HAlign_Center);
}

UBaseButton* URPGPauseActionsWidget::BuildTextButton(FName WidgetName, const FString& Label)
{
	UBaseButton* Button = WidgetTree->ConstructWidget<UBaseButton>(UBaseButton::StaticClass(), WidgetName);
	Button->SetText(Label);
	return Button;
}

UTextBlock* URPGPauseActionsWidget::BuildText(FName WidgetName, const FString& Label, int32 FontSize) const
{
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), WidgetName);
	Text->SetText(FText::FromString(Label));
	Text->SetColorAndOpacity(FSlateColor(FLinearColor(0.82f, 0.72f, 0.52f, 1.f)));

	FSlateFontInfo Font = Text->GetFont();
	Font.Size = FontSize;
	Text->SetFont(Font);

	return Text;
}
