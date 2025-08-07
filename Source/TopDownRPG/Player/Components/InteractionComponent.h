// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputAction.h"
#include "RPGActorComponentBase.h"
#include "Camera/CameraComponent.h"
#include "Components/ActorComponent.h"
#include "TopDownRPG/TopDownRPG.h"
#include "TopDownRPG/Interfaces/InteractableInterface.h"
#include "TopDownRPG/UI/HUD/PlayerHUD.h"
#include "InteractionComponent.generated.h"


UCLASS(ClassGroup=(PLayerComponents), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API UInteractionComponent : public URPGActorComponentBase
{
	GENERATED_BODY()
	
public:
	UInteractionComponent();
	virtual void SetupComponent(UPlayerSettings* Settings) override;
	virtual void Dispose() override;
	
protected:
	UPROPERTY(Transient)
	UCameraComponent* PlayerCamera;
	
	UPROPERTY(EditDefaultsOnly)
	UInputAction* InteractAction;
	
	TWeakInterfacePtr<IInteractableInterface> InteractionTarget;

	ECharacterState CurrentState = Dead;
	
	// Called when the game starts
	virtual void BeginPlay() override;
	void SetInteractionTarget(IInteractableInterface* Interactable);
	UFUNCTION()
	void OnInteract();

	UFUNCTION()
	void OnStateChanged(ECharacterState State);
public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
};
