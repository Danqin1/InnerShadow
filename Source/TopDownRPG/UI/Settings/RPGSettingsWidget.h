#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "RPGSettingsWidget.generated.h"

class USlider;
class UTextBlock;
class UVerticalBox;
class SWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRPGSettingsValueChanged, float, Value);

UCLASS()
class TOPDOWNRPG_API URPGSettingsWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings", meta=(ClampMin="0.0", ClampMax="1.0"))
	float DefaultMasterVolume = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings", meta=(ClampMin="0.0", ClampMax="1.0"))
	float DefaultMusicVolume = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Settings", meta=(ClampMin="0.0", ClampMax="1.0"))
	float DefaultSFXVolume = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* MasterVolumeValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* MusicVolumeValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* SFXVolumeValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	USlider* MasterVolumeSlider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	USlider* MusicVolumeSlider;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	USlider* SFXVolumeSlider;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleMasterVolumeChanged(float Value);

	UFUNCTION()
	void HandleMusicVolumeChanged(float Value);

	UFUNCTION()
	void HandleSFXVolumeChanged(float Value);

private:
	float MasterVolume = 1.f;
	float MusicVolume = 1.f;
	float SFXVolume = 1.f;
	bool bApplyingValues = false;
	bool bHasExplicitValues = false;

	void BuildDefaultLayout();
	UTextBlock* BuildText(FName WidgetName, const FString& Label, int32 FontSize) const;
	void AddSettingRow(UVerticalBox* Parent, const FString& Label, USlider*& Slider, UTextBlock*& ValueText);
	void BindControls();
	void UnbindControls();
	void ApplyVolumeValuesToControls();
	bool LoadVolumeValuesFromSave();
	void SaveVolumeValues();
	void SetSettingValueText(UTextBlock* ValueText, float Value) const;

public:
	UPROPERTY(BlueprintAssignable, Category="Settings")
	FRPGSettingsValueChanged OnMasterVolumeChanged;

	UPROPERTY(BlueprintAssignable, Category="Settings")
	FRPGSettingsValueChanged OnMusicVolumeChanged;

	UPROPERTY(BlueprintAssignable, Category="Settings")
	FRPGSettingsValueChanged OnSFXVolumeChanged;

	UFUNCTION(BlueprintCallable, Category="Settings")
	void SetVolumeValues(float InMasterVolume, float InMusicVolume, float InSFXVolume, bool bBroadcastChanges);

	UFUNCTION(BlueprintPure, Category="Settings")
	float GetMasterVolume() const { return MasterVolume; }

	UFUNCTION(BlueprintPure, Category="Settings")
	float GetMusicVolume() const { return MusicVolume; }

	UFUNCTION(BlueprintPure, Category="Settings")
	float GetSFXVolume() const { return SFXVolume; }
};
