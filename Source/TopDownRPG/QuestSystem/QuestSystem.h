// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "QuestsDatabase.h"
#include "TopDownRPG/Save/SaveData.h"
#include "QuestSystem.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UQuestSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
protected:
	UPROPERTY()
	UQuestsDatabase* QuestsDatabase = nullptr;
	
	UPROPERTY()
	USaveData* SaveData;
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void RestoreFromSave(USaveData* save);

	UFUNCTION(BlueprintCallable)
	void StartQuest(int QuestID);

	UFUNCTION(BlueprintCallable)
	const TArray<FQuest>& GetActiveQuests() const { return SaveData->ActiveQuests; }

	UFUNCTION(BlueprintCallable)
	const TArray<int>& GetCompletedQuests() const { return SaveData->CompletedQuests; }

	UFUNCTION(BlueprintCallable)
	TArray<FQuest> GetQuestsForNPC(int NPCID);
};
