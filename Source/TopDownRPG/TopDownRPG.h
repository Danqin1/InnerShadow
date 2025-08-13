// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTopDownRPG, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBoolEvent, bool, isTrue);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStringEvent, FString, string);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FFloatEvent, float, value);

template<typename T>
static FString EnumToString(const FString& enumName, const T value)
{
	UEnum* pEnum = FindFirstObjectSafe<UEnum>(*enumName);
	return *(pEnum ? pEnum->GetNameStringByIndex(static_cast<uint8>(value)) : "null");
}

UENUM(BlueprintType)
enum ECastType
{
	None,
	Direction,
	Target
};

UENUM(BlueprintType)
enum ECharacterState
{
	Nothing,
	Chasing,
	Attacking,
	Interaction,
	Cutscene,
	Dead,
	Skill
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FStateChanged, ECharacterState, State);

UENUM()
enum EMovementType
{
	Walk,
	Run,
};

USTRUCT(BlueprintType)
struct FAbilityData
{
	GENERATED_BODY()
public:
	int ID = 0;
	bool bIsUnlocked = false;
	int Level = 0;
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAbityEvent, FAbilityData, value);