// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TopDownRPG/Enemy/Base/EnemyCharacterBase.h"
#include "Shadow.generated.h"

UCLASS()
class TOPDOWNRPG_API AShadow : public AEnemyCharacterBase
{
	GENERATED_BODY()

public:
	AShadow();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
