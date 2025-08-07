// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/Items/ItemBase.h"
#include "TopDownRPG/Interfaces/InteractableInterface.h"
#include "TopDownRPG/Player/RPGCharacter.h"
#include "TopDownRPG/Player/RPGPlayerController.h"
#include "InteractableBase.generated.h"

UCLASS()
class TOPDOWNRPG_API AInteractableBase : public AItemBase, public IInteractableInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AInteractableBase();

protected:
	UPROPERTY(Transient)
	ARPGCharacter* InteractCharacter;
	
	UPROPERTY(EditDefaultsOnly, Category="Mesh")
	UStaticMeshComponent* MeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	float MaxInteractionRange = 200;
	UPROPERTY(EditDefaultsOnly, Category="Hover")
	UWidgetComponent* InteractionWidgetComponent;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	ARPGPlayerController* GetPlayerController();

public:
	virtual void Interact(ACharacter* Character) override;
	virtual void InteractionFinished() override;
	virtual void SetAsTarget(bool isTargeted) override;
};
