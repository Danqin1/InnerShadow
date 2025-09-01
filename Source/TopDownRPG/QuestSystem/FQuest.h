#pragma once

#include "FQuest.generated.h"

USTRUCT(BlueprintType)
struct FQuestObjective
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Description;

	UPROPERTY(EditAnywhere)
	bool bIsCompleted = false;
};

USTRUCT(BlueprintType)
struct FQuest
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int ID = -1;
	
	UPROPERTY(EditAnywhere)
	FString Name = "";
	UPROPERTY(EditAnywhere)
	FString Description = "";

	UPROPERTY(EditAnywhere)
	TArray<FQuestObjective> Objectives;
	
	UPROPERTY(EditAnywhere)
	bool IsSecondary = false;
	
	UPROPERTY(EditAnywhere)
	bool IsCompleted = false;

	FQuest()
	{
		ID = static_cast<int>(GetTypeHash(FGuid::NewGuid()));
	}
};