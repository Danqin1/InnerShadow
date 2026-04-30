#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TopDownRPG/Progression/RunUpgradeTypes.h"
#include "RunUpgradeChoiceCard.generated.h"

class UButton;
class UTextBlock;
class SWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRunUpgradeChoiceCardSelected, FRunUpgradeData, Upgrade);

UCLASS()
class TOPDOWNRPG_API URunUpgradeChoiceCard : public UUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UButton* SelectButton;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* NameText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* DescriptionText;

	UPROPERTY(BlueprintReadOnly, Category="Run Upgrade")
	FRunUpgradeData UpgradeData;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	UFUNCTION()
	void HandleClicked();

private:
	void BuildDefaultLayout();
	void RefreshDisplay();

public:
	UPROPERTY(BlueprintAssignable, Category="Run Upgrade")
	FRunUpgradeChoiceCardSelected OnSelected;

	UFUNCTION(BlueprintCallable, Category="Run Upgrade")
	void SetUpgradeData(FRunUpgradeData InUpgradeData);

	UFUNCTION(BlueprintPure, Category="Run Upgrade")
	FRunUpgradeData GetUpgradeData() const;
};
