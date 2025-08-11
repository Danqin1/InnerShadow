// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CommonButtonBase.h"
#include "CommonUI/Public/CommonTextBlock.h"
#include "BaseButton.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNRPG_API UBaseButton : public UCommonButtonBase
{
	GENERATED_BODY()
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, meta=(BindWidget))
	UCommonTextBlock* ButtonText;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Text;

	UFUNCTION(BlueprintCallable)
	void SetText(FString newText);
};
