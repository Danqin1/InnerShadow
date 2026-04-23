#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TopDownRPG/Progression/RunUpgradeTypes.h"
#include "UpgradesDatabase.generated.h"

UCLASS()
class TOPDOWNRPG_API UUpgradesDatabase : public UDataAsset
{
	GENERATED_BODY()

public:
	UUpgradesDatabase();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Upgrades")
	TArray<FRunUpgradeData> Upgrades;

	const FRunUpgradeData* GetUpgradeByID(FName UpgradeID) const;
};
