// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "W_ActionSlot.generated.h"

/**
 * 
 */
UCLASS(HideDropdown)
class TOPDOWNRPG_API UW_ActionSlot : public UUserWidget
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UImage* Icon;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UProgressBar* CooldownOverlay;
	UPROPERTY(EditDefaultsOnly, meta=(BindWidget))
	UTextBlock* Name;

	void UpdateRecharge(float progress);
};
