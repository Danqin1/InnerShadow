#pragma once

#include "FKillPrize.generated.h"

USTRUCT(BlueprintType)
struct FKillPrize
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int XP  = 1;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Darkness  = 5;
};
