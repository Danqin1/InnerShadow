// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "TopDownRPG/Interfaces/InteractableInterface.h"
#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/UI/Quests/UW_QuestGIver.h"
#include "QuestGiver.generated.h"

UCLASS()
class TOPDOWNRPG_API AQuestGiver : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AQuestGiver();

protected:
	UPROPERTY(Transient)
	ARPGCharacter* InteractCharacter;

	UPROPERTY(Transient)
	UUW_QuestGIver* UIWidget;
	
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* InteractionWidget;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUW_QuestGIver> UIClass;

	UPROPERTY(EditAnywhere, Category="Quests") // TODO: Change to use database
	TArray<FQuest> Quests;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	virtual void Interact(ACharacter* Character) override;
	UFUNCTION()
	virtual void InteractionFinished() override;
	UFUNCTION()
	virtual void SetAsTarget(bool isTargeted) override;
};
