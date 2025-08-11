// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/QuestSystem/FQuest.h"
#include "CommonUI/Public/CommonUserWidget.h"
#include "TopDownRPG/UI/Common/BaseButton.h"
#include "UW_QuestSlot.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FQuestSlotClicked, FQuest, Quest);

UCLASS(HideDropdown)
class TOPDOWNRPG_API UUW_QuestSlot : public UCommonUserWidget
{
	GENERATED_BODY()

	FQuest TargetQuest;
public:
	FQuestSlotClicked OnClicked;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UBaseButton* Button;

	UFUNCTION(BlueprintCallable)
	void OnClick();
	
	virtual void NativeConstruct() override;

	void Populate(FQuest Quest);
};
