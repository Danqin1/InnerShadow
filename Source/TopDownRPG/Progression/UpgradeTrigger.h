// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "GameFramework/Actor.h"
#include "UpgradeTrigger.generated.h"

class URunUpgradeChoiceUI;

UCLASS()
class TOPDOWNRPG_API AUpgradeTrigger : public AActor
{
	GENERATED_BODY()

public:
	AUpgradeTrigger();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Upgrade Trigger", meta=(AllowPrivateAccess="true"))
	TObjectPtr<USceneComponent> SceneRoot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Upgrade Trigger", meta=(AllowPrivateAccess="true"))
	TObjectPtr<UBoxComponent> TriggerBox;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Upgrade Trigger")
	FName TriggerSaveID;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Upgrade Trigger")
	TSubclassOf<URunUpgradeChoiceUI> RunUpgradeChoiceUIClass;

	UPROPERTY(Transient)
	TObjectPtr<URunUpgradeChoiceUI> ActiveUpgradeUI;

	UFUNCTION()
	void HandleTriggerBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	void DisableTrigger();
	FName GetResolvedTriggerSaveID() const;
	bool IsTriggerAlreadyUsed() const;
	bool TryShowUpgradeUI();
};
