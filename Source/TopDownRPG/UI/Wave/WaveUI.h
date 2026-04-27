// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Widgets/SWidget.h"
#include "WaveUI.generated.h"

/**
 * Runtime wave encounter display.
 */
UCLASS()
class TOPDOWNRPG_API UWaveUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidgetOptional))
	UTextBlock* WavesLeftText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidgetOptional))
	UTextBlock* NextWaveTimerText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidgetOptional))
	UProgressBar* NextWaveTimerBar;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:
	void BuildDefaultLayout();

public:
	UFUNCTION(BlueprintCallable)
	void SetWavesLeft(int32 WavesLeft);

	UFUNCTION(BlueprintCallable)
	void SetNextWaveTimer(float TimeRemaining, float TotalDuration);

	UFUNCTION(BlueprintCallable)
	void SetNextWaveTimerVisible(bool bVisible);
	
	UFUNCTION(BlueprintImplementableEvent)
	void FadeOutAndDestroy();
};
