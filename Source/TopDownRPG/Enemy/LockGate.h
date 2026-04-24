// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "LockGate.generated.h"

UCLASS()
class TOPDOWNRPG_API ALockGate : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALockGate();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Open();
	
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void Close();
};
