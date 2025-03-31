// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogTopDownRPG, Log, All);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FDynamicEvent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FBoolEvent, bool, isTrue);

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
	Dead
};