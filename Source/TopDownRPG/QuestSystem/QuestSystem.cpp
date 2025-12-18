// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestSystem.h"

#include "TopDownRPG/DevDebug.h"

void UQuestSystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	if (UQuestsDatabase* DB = LoadObject<UQuestsDatabase>(nullptr, TEXT("/Game/Database/DA_QuestsDatabase")))
	{
		QuestsDatabase = DB;
	}
	if (!QuestsDatabase)
	{
		DevDebug::OnScreenLog("QuestsDatabase is not set", FColor::Red, 5.f);
	}
}

void UQuestSystem::RestoreFromSave(USaveData* save)
{
	SaveData = save;
}

void UQuestSystem::StartQuest(int QuestID)
{
	if (FQuest* quest = QuestsDatabase->GetQuestByID(QuestID))
	{
		if (!SaveData->ActiveQuests.ContainsByPredicate([QuestID](const FQuest item){return item.ID == QuestID;}) && !SaveData->CompletedQuests.Contains(QuestID))
		{
			SaveData->ActiveQuests.Add(*quest);
		}
	}
}

TArray<FQuest> UQuestSystem::GetQuestsForNPC(int NPCID)
{
	return QuestsDatabase->GetNPCQuests(NPCID);
}
