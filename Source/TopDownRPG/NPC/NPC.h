// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "TopDownRPG/Interfaces/InteractableInterface.h"
#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/UI/Quests/UW_NPC.h"
#include "NPC.generated.h"

UCLASS()
class TOPDOWNRPG_API ANPC : public ACharacter, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ANPC();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "NPC")
	int NPCID = -1;
	
	UPROPERTY(Transient)
	ARPGCharacter* InteractCharacter;

	UPROPERTY(Transient)
	UUW_NPC* UIWidget;
	
	UPROPERTY(EditDefaultsOnly)
	UWidgetComponent* InteractionWidget;

	UPROPERTY(EditDefaultsOnly, Category="UI")
	TSubclassOf<UUW_NPC> UIClass;

	UPROPERTY(VisibleAnywhere, Category="Quests")
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
