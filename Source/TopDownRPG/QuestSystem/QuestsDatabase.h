// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "FQuest.h"
#include "Engine/DataAsset.h"
#include "QuestsDatabase.generated.h"

USTRUCT(BlueprintType)
struct FNPCQuests
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quests")
	int NPCID = -1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Quests")
	TArray<FQuest> Quests;
};
/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UQuestsDatabase : public UDataAsset
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Quests")
	TArray<FNPCQuests> NPCQuests;
public:
	
	FQuest* GetQuestByID(int QuestID);

	UFUNCTION(BlueprintCallable, Category="Quests")
	TArray<FQuest> GetNPCQuests(int NPCID);
};
