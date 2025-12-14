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

USTRUCT(BlueprintType)
struct FLocationObjective : public FQuestObjective
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	FVector Location;

	UPROPERTY(EditAnywhere)
	float Radius = 300;
};

USTRUCT(BlueprintType)
struct FFindObjective : public FQuestObjective
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ObjectToFind;
};