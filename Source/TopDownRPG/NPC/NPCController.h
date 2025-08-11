// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "NPCController.generated.h"

UCLASS()
class TOPDOWNRPG_API ANPCController : public AAIController
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ANPCController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
