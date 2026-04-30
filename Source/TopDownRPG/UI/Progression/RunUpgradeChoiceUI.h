#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownRPG/Progression/RunUpgradeTypes.h"
#include "RunUpgradeChoiceUI.generated.h"

class URunUpgradeChoiceCard;
class UHorizontalBox;
class UTextBlock;
class SWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRunUpgradeSelected, FRunUpgradeData, Upgrade);

UCLASS()
class TOPDOWNRPG_API URunUpgradeChoiceUI : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Run Upgrade")
	int32 ChoiceCount = 3;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Run Upgrade")
	bool bRemoveFromParentOnSelection = true;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Run Upgrade")
	TSubclassOf<URunUpgradeChoiceCard> CardClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UHorizontalBox* ChoicesBox;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* TitleText;

	UPROPERTY(Transient)
	TArray<URunUpgradeChoiceCard*> ChoiceCards;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleUpgradeSelected(FRunUpgradeData Upgrade);

private:
	bool bSelectionLocked = false;

	void BuildDefaultLayout();
	void ClearChoiceCards();
	void SetGameplayInputEnabled(bool bEnabled) const;

public:
	UPROPERTY(BlueprintAssignable, Category="Run Upgrade")
	FRunUpgradeSelected OnUpgradeSelected;

	UFUNCTION(BlueprintCallable, Category="Run Upgrade")
	bool RefreshChoices();
};
