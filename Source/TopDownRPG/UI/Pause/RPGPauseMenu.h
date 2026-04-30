#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "RPGPauseMenu.generated.h"

class UButton;
class UTextBlock;
class UWidget;
class UWidgetSwitcher;
class URPGPauseActionsWidget;
class URPGSettingsWidget;
class URPGStatsWidget;
class SWidget;

UENUM(BlueprintType)
enum class EPauseMenuTab : uint8
{
	Pause,
	Stats,
	Settings
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPauseMenuTabChanged, EPauseMenuTab, Tab);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPauseMenuSettingChanged, float, Value);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseMenuSimpleEvent);

UCLASS()
class TOPDOWNRPG_API URPGPauseMenu : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pause Menu")
	EPauseMenuTab DefaultTab = EPauseMenuTab::Pause;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pause Menu")
	bool bPauseGameOnOpen = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pause Menu")
	bool bRestoreGameInputOnClose = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pause Menu|Tabs")
	TSubclassOf<URPGPauseActionsWidget> PauseActionsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pause Menu|Tabs")
	TSubclassOf<URPGStatsWidget> StatsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Pause Menu|Tabs")
	TSubclassOf<URPGSettingsWidget> SettingsWidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* PauseTabButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* StatsTabButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* SettingsTabButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UWidgetSwitcher* TabSwitcher;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	URPGPauseActionsWidget* PauseActionsWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	URPGStatsWidget* StatsWidget;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	URPGSettingsWidget* SettingsWidget;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandlePauseTabClicked();

	UFUNCTION()
	void HandleStatsTabClicked();

	UFUNCTION()
	void HandleSettingsTabClicked();

	UFUNCTION()
	void HandleResumeClicked();

	UFUNCTION()
	void HandleQuitClicked();

	UFUNCTION()
	void HandleMasterVolumeChanged(float Value);

	UFUNCTION()
	void HandleMusicVolumeChanged(float Value);

	UFUNCTION()
	void HandleSFXVolumeChanged(float Value);

private:
	bool bPreviousShowMouseCursor = false;
	bool bAppliedPauseState = false;
	bool bAppliedMenuInput = false;

	void BuildDefaultLayout();
	UButton* BuildTextButton(FName WidgetName, const FString& Label);
	UTextBlock* BuildText(FName WidgetName, const FString& Label, int32 FontSize) const;
	void RebuildTabPages();
	void AddTabPage(UWidget* PageWidget);
	void BindControls();
	void UnbindControls();
	void ApplyMenuInput();
	void RestoreGameInput();

public:
	UPROPERTY(BlueprintAssignable, Category="Pause Menu")
	FPauseMenuTabChanged OnTabChanged;

	UPROPERTY(BlueprintAssignable, Category="Pause Menu")
	FPauseMenuSimpleEvent OnResumeRequested;

	UPROPERTY(BlueprintAssignable, Category="Pause Menu")
	FPauseMenuSimpleEvent OnQuitRequested;

	UPROPERTY(BlueprintAssignable, Category="Pause Menu")
	FPauseMenuSettingChanged OnMasterVolumeChanged;

	UPROPERTY(BlueprintAssignable, Category="Pause Menu")
	FPauseMenuSettingChanged OnMusicVolumeChanged;

	UPROPERTY(BlueprintAssignable, Category="Pause Menu")
	FPauseMenuSettingChanged OnSFXVolumeChanged;

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void SelectTab(EPauseMenuTab Tab);

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void RefreshStats();

	UFUNCTION(BlueprintCallable, Category="Pause Menu")
	void CloseMenu();
};
