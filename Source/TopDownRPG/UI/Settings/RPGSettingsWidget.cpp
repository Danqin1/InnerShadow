#include "RPGSettingsWidget.h"

#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Components/VerticalBoxSlot.h"
#include "TopDownRPG/Save/SaveSystem.h"
#include "Widgets/SWidget.h"

TSharedRef<SWidget> URPGSettingsWidget::RebuildWidget()
{
	BuildDefaultLayout();
	return Super::RebuildWidget();
}

void URPGSettingsWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (!LoadVolumeValuesFromSave() && !bHasExplicitValues)
	{
		MasterVolume = FMath::Clamp(DefaultMasterVolume, 0.f, 1.f);
		MusicVolume = FMath::Clamp(DefaultMusicVolume, 0.f, 1.f);
		SFXVolume = FMath::Clamp(DefaultSFXVolume, 0.f, 1.f);
	}

	BindControls();
	ApplyVolumeValuesToControls();
}

void URPGSettingsWidget::NativeDestruct()
{
	UnbindControls();
	Super::NativeDestruct();
}

void URPGSettingsWidget::SetVolumeValues(float InMasterVolume, float InMusicVolume, float InSFXVolume, bool bBroadcastChanges)
{
	MasterVolume = FMath::Clamp(InMasterVolume, 0.f, 1.f);
	MusicVolume = FMath::Clamp(InMusicVolume, 0.f, 1.f);
	SFXVolume = FMath::Clamp(InSFXVolume, 0.f, 1.f);
	bHasExplicitValues = true;

	ApplyVolumeValuesToControls();

	if (bBroadcastChanges)
	{
		OnMasterVolumeChanged.Broadcast(MasterVolume);
		OnMusicVolumeChanged.Broadcast(MusicVolume);
		OnSFXVolumeChanged.Broadcast(SFXVolume);
	}
}

void URPGSettingsWidget::HandleMasterVolumeChanged(float Value)
{
	MasterVolume = FMath::Clamp(Value, 0.f, 1.f);
	SetSettingValueText(MasterVolumeValueText, MasterVolume);

	if (!bApplyingValues)
	{
		SaveVolumeValues();
		OnMasterVolumeChanged.Broadcast(MasterVolume);
	}
}

void URPGSettingsWidget::HandleMusicVolumeChanged(float Value)
{
	MusicVolume = FMath::Clamp(Value, 0.f, 1.f);
	SetSettingValueText(MusicVolumeValueText, MusicVolume);

	if (!bApplyingValues)
	{
		SaveVolumeValues();
		OnMusicVolumeChanged.Broadcast(MusicVolume);
	}
}

void URPGSettingsWidget::HandleSFXVolumeChanged(float Value)
{
	SFXVolume = FMath::Clamp(Value, 0.f, 1.f);
	SetSettingValueText(SFXVolumeValueText, SFXVolume);

	if (!bApplyingValues)
	{
		SaveVolumeValues();
		OnSFXVolumeChanged.Broadcast(SFXVolume);
	}
}

void URPGSettingsWidget::BuildDefaultLayout()
{
	if (!WidgetTree || WidgetTree->RootWidget)
	{
		return;
	}

	UVerticalBox* Root = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), TEXT("SettingsRoot"));
	WidgetTree->RootWidget = Root;

	AddSettingRow(Root, TEXT("Master Volume"), MasterVolumeSlider, MasterVolumeValueText);
	AddSettingRow(Root, TEXT("Music Volume"), MusicVolumeSlider, MusicVolumeValueText);
	AddSettingRow(Root, TEXT("SFX Volume"), SFXVolumeSlider, SFXVolumeValueText);
}

UTextBlock* URPGSettingsWidget::BuildText(FName WidgetName, const FString& Label, int32 FontSize) const
{
	UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), WidgetName);
	Text->SetText(FText::FromString(Label));
	Text->SetColorAndOpacity(FSlateColor(FLinearColor(0.82f, 0.72f, 0.52f, 1.f)));

	FSlateFontInfo Font = Text->GetFont();
	Font.Size = FontSize;
	Text->SetFont(Font);

	return Text;
}

void URPGSettingsWidget::AddSettingRow(UVerticalBox* Parent, const FString& Label, USlider*& Slider, UTextBlock*& ValueText)
{
	FString SafeLabel = Label;
	SafeLabel.ReplaceInline(TEXT(" "), TEXT(""));

	UHorizontalBox* Row = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass(), FName(*(SafeLabel + TEXT("Row"))));
	UVerticalBoxSlot* RowSlot = Parent->AddChildToVerticalBox(Row);
	RowSlot->SetHorizontalAlignment(HAlign_Fill);
	RowSlot->SetPadding(FMargin(0.f, 0.f, 0.f, 18.f));

	UTextBlock* LabelText = BuildText(FName(*(SafeLabel + TEXT("Label"))), Label, 18);
	UHorizontalBoxSlot* LabelSlot = Row->AddChildToHorizontalBox(LabelText);
	LabelSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));

	Slider = WidgetTree->ConstructWidget<USlider>(USlider::StaticClass(), FName(*(SafeLabel + TEXT("Slider"))));
	Slider->SetValue(1.f);
	UHorizontalBoxSlot* SliderSlot = Row->AddChildToHorizontalBox(Slider);
	SliderSlot->SetSize(FSlateChildSize(ESlateSizeRule::Fill));
	SliderSlot->SetPadding(FMargin(16.f, 0.f));

	ValueText = BuildText(FName(*(SafeLabel + TEXT("Value"))), TEXT("100%"), 18);
	ValueText->SetJustification(ETextJustify::Right);
	UHorizontalBoxSlot* ValueSlot = Row->AddChildToHorizontalBox(ValueText);
	ValueSlot->SetSize(FSlateChildSize(ESlateSizeRule::Automatic));
}

void URPGSettingsWidget::BindControls()
{
	if (MasterVolumeSlider) { MasterVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &URPGSettingsWidget::HandleMasterVolumeChanged); }
	if (MusicVolumeSlider) { MusicVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &URPGSettingsWidget::HandleMusicVolumeChanged); }
	if (SFXVolumeSlider) { SFXVolumeSlider->OnValueChanged.AddUniqueDynamic(this, &URPGSettingsWidget::HandleSFXVolumeChanged); }
}

void URPGSettingsWidget::UnbindControls()
{
	if (MasterVolumeSlider) { MasterVolumeSlider->OnValueChanged.RemoveDynamic(this, &URPGSettingsWidget::HandleMasterVolumeChanged); }
	if (MusicVolumeSlider) { MusicVolumeSlider->OnValueChanged.RemoveDynamic(this, &URPGSettingsWidget::HandleMusicVolumeChanged); }
	if (SFXVolumeSlider) { SFXVolumeSlider->OnValueChanged.RemoveDynamic(this, &URPGSettingsWidget::HandleSFXVolumeChanged); }
}

void URPGSettingsWidget::ApplyVolumeValuesToControls()
{
	bApplyingValues = true;

	if (MasterVolumeSlider)
	{
		MasterVolumeSlider->SetValue(MasterVolume);
	}
	SetSettingValueText(MasterVolumeValueText, MasterVolume);

	if (MusicVolumeSlider)
	{
		MusicVolumeSlider->SetValue(MusicVolume);
	}
	SetSettingValueText(MusicVolumeValueText, MusicVolume);

	if (SFXVolumeSlider)
	{
		SFXVolumeSlider->SetValue(SFXVolume);
	}
	SetSettingValueText(SFXVolumeValueText, SFXVolume);

	bApplyingValues = false;
}

bool URPGSettingsWidget::LoadVolumeValuesFromSave()
{
	if (!GetGameInstance())
	{
		return false;
	}

	USaveSystem* SaveSystem = GetGameInstance()->GetSubsystem<USaveSystem>();
	if (!SaveSystem)
	{
		return false;
	}

	const USaveData* SaveData = SaveSystem->GetSaveData();
	if (!SaveData)
	{
		return false;
	}

	MasterVolume = FMath::Clamp(SaveData->PlayerSettings.MasterVolume, 0.f, 1.f);
	MusicVolume = FMath::Clamp(SaveData->PlayerSettings.MusicVolume, 0.f, 1.f);
	SFXVolume = FMath::Clamp(SaveData->PlayerSettings.SFXVolume, 0.f, 1.f);
	bHasExplicitValues = true;
	return true;
}

void URPGSettingsWidget::SaveVolumeValues()
{
	if (!GetGameInstance())
	{
		return;
	}

	if (USaveSystem* SaveSystem = GetGameInstance()->GetSubsystem<USaveSystem>())
	{
		SaveSystem->SetPlayerAudioSettings(MasterVolume, MusicVolume, SFXVolume, true);
	}
}

void URPGSettingsWidget::SetSettingValueText(UTextBlock* ValueText, float Value) const
{
	if (ValueText)
	{
		ValueText->SetText(FText::FromString(FString::Printf(TEXT("%.0f%%"), FMath::Clamp(Value, 0.f, 1.f) * 100.f)));
	}
}
