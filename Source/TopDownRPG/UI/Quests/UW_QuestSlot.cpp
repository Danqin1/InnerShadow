// Fill out your copyright notice in the Description page of Project Settings.


#include "UW_QuestSlot.h"

void UUW_QuestSlot::NativeConstruct()
{
	Super::NativeConstruct();
}

void UUW_QuestSlot::Populate(FQuest Quest)
{
	TargetQuest = Quest;
	Button->SetText(Quest.Name);
}

void UUW_QuestSlot::OnClick()
{
	if(OnClicked.IsBound())
	{
		OnClicked.Broadcast(TargetQuest);
	}
}
