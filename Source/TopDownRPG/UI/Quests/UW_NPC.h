// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UW_QuestSlot.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/QuestSystem/FQuest.h"
#include "CommonUI/Public/CommonUserWidget.h"
#include "CommonUI/Public/CommonButtonBase.h"
#include "UW_NPC.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UUW_NPC : public UCommonUserWidget
{
	GENERATED_BODY()

	UPROPERTY(Transient)
	TArray<UUW_QuestSlot*> TargetSlots;

	UPROPERTY(Transient)
	TArray<FQuest> TargetQuests;

	FQuest CurrentShowing;
protected:
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UVerticalBox* QuestsList;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	UTextBlock* QuestDescription;

	UPROPERTY(EditAnywhere, Category="Slots")
	TSubclassOf<UUW_QuestSlot> SlotClass;

	UFUNCTION(BlueprintCallable)
	void OnCloseButton();
	UFUNCTION(BlueprintCallable)
	void OnSelectButton();
	UFUNCTION(BlueprintCallable)
	void OnQuestClicked(FQuest Quest);
public:
	FDynamicEvent OnClose;
	
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	void Populate(TArray<FQuest> Quests);
};
