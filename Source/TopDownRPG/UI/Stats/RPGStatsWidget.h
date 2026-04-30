#pragma once

#include "CoreMinimal.h"
#include "CommonUserWidget.h"
#include "RPGStatsWidget.generated.h"

class UPlayerStatsComponent;
class UTextBlock;
class UVerticalBox;
class SWidget;

UCLASS()
class TOPDOWNRPG_API URPGStatsWidget : public UCommonUserWidget
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* HealthValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* EssenceValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* EssenceStateValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* DamageValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* IncomingDamageValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* AttackSpeedValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* HealthRegenValueText;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta=(BindWidget))
	UTextBlock* EssenceGainValueText;

	UPROPERTY(Transient, BlueprintReadOnly, Category="Stats")
	UPlayerStatsComponent* StatsComponentOverride = nullptr;

	virtual TSharedRef<SWidget> RebuildWidget() override;
	virtual void NativeConstruct() override;

private:
	void BuildDefaultLayout();
	UTextBlock* BuildText(FName WidgetName, const FString& Label, int32 FontSize) const;
	void AddStatRow(UVerticalBox* Parent, const FString& Label, UTextBlock*& ValueText);
	UPlayerStatsComponent* FindPlayerStatsComponent() const;
	FString FormatPercentMultiplier(float Multiplier) const;
	FString GetEssenceStateText(UPlayerStatsComponent* StatsComponent) const;

public:
	UFUNCTION(BlueprintCallable, Category="Stats")
	void SetStatsComponent(UPlayerStatsComponent* InStatsComponent);

	UFUNCTION(BlueprintCallable, Category="Stats")
	void RefreshStats();
};
