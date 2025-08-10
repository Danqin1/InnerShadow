// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "W_ActionSlot.h"
#include "Blueprint/UserWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/ProgressBar.h"
#include "Components/SizeBox.h"
#include "TopDownRPG/TopDownRPG.h"
#include "PlayerHUD.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class TOPDOWNRPG_API UPlayerHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UProgressBar* HealthBar;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UProgressBar* EssenceBar;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UProgressBar* RageBar;
	
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UImage* EssenceIndicator;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UW_ActionSlot* Action_0;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UW_ActionSlot* Action_1;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UW_ActionSlot* Action_2;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UW_ActionSlot* Action_3;

	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* StateDebug;
	
public:
	void SetHP(float Percent);
	void SetAction(int slot, FString name, UTexture2D* icon);
	void StateChanged(ECharacterState State);

	UW_ActionSlot* GetUISlot(int index);
	void SetEssence(float X);
	void SetRage(float X);
};
