// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_NPC.h"

#include "Kismet/GameplayStatics.h"
#include "TopDownRPG/DevDebug.h"

void UUW_NPC::NativeConstruct()
{
	auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if(PlayerController)
	{
		PlayerController->SetShowMouseCursor(true);
		PlayerController->SetInputMode(FInputModeGameAndUI());
	}
	
	Super::NativeConstruct();
}

void UUW_NPC::NativeDestruct()
{
	auto* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	if(PlayerController)
	{
		PlayerController->SetShowMouseCursor(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	Super::NativeDestruct();
}

void UUW_NPC::Populate(TArray<FQuest> Quests)
{
	TargetQuests = Quests;
	for (FQuest Quest : Quests)
	{
		if (SlotClass)
		{
			auto* QuestSlot = CreateWidget<UUW_QuestSlot>(GetWorld(), SlotClass);
			QuestsList->AddChild(QuestSlot);
			QuestSlot->Populate(Quest);
			TargetSlots.Add(QuestSlot);
			QuestSlot->OnClicked.AddDynamic(this, &UUW_NPC::OnQuestClicked);
		}
		else
		{
			DevDebug::OnScreenLog("SlotClass is not set for UUW_NPC", FColor::Red, 5.f);
		}
	}
}

void UUW_NPC::OnQuestClicked(FQuest Quest)
{
	CurrentShowing = Quest;
	QuestDescription->SetText(FText::FromString(Quest.Description));
}

void UUW_NPC::OnCloseButton()
{
	for (UUW_QuestSlot* QuestSlot : TargetSlots)
	{
		QuestSlot->OnClicked.RemoveDynamic(this, &UUW_NPC::OnQuestClicked);;
	}
	
	if(OnClose.IsBound())
	{
		OnClose.Broadcast();
	}
}

void UUW_NPC::OnSelectButton()
{
}
