#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "TopDownRPG/Progression/RunUpgradeTypes.h"
#include "RunUpgradeSystem.generated.h"

class USaveData;
class UUpgradesDatabase;

UCLASS()
class TOPDOWNRPG_API URunUpgradeSystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	USaveData* SaveData = nullptr;

	UPROPERTY()
	UUpgradesDatabase* UpgradesDatabase = nullptr;

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	void RestoreFromSave(USaveData* InSaveData);

	UFUNCTION(BlueprintCallable)
	TArray<FRunUpgradeData> GetRandomUpgradeChoices(int32 Count = 5) const;

	UFUNCTION(BlueprintCallable)
	bool ApplyUpgradeByID(FName UpgradeID);

	UFUNCTION(BlueprintCallable)
	TArray<FName> GetTakenUpgradeIDs() const;
};
