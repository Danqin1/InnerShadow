// Fill out your copyright notice in the Description page of Project Settings.


#include "QuestsDatabase.h"

FQuest* UQuestsDatabase::GetQuestByID(int QuestID)
{
	for (FNPCQuests NPCQuest : NPCQuests)
	{
		if (FQuest* quest = NPCQuest.Quests.FindByPredicate([QuestID](const FQuest item){return item.ID == QuestID;}))
		{
			return quest;
		}
	}
	return nullptr;
}

TArray<FQuest> UQuestsDatabase::GetNPCQuests(int NPCID)
{
	if (NPCID == -1)
	{
		return {};
	}
	for (FNPCQuests NPCQuest : NPCQuests)
	{
		if (NPCQuest.NPCID == NPCID)
		{
			return NPCQuest.Quests;
		}
	}
	return {};
}
