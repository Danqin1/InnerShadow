#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "TopDownRPG/UI/Common/BaseButton.h"
#include "RPGPauseActionsWidget.generated.h"

class UTextBlock;
class SWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPauseActionWidgetEvent);

UCLASS()
class TOPDOWNRPG_API URPGPauseActionsWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UBaseButton* ResumeButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UBaseButton* QuitButton;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleResumeClicked();

	UFUNCTION()
	void HandleQuitClicked();

private:
	void BuildDefaultLayout();
	UBaseButton* BuildTextButton(FName WidgetName, const FString& Label);
	UTextBlock* BuildText(FName WidgetName, const FString& Label, int32 FontSize) const;

public:
	UPROPERTY(BlueprintAssignable, Category="Pause")
	FPauseActionWidgetEvent OnResumeRequested;

	UPROPERTY(BlueprintAssignable, Category="Pause")
	FPauseActionWidgetEvent OnQuitRequested;
};
