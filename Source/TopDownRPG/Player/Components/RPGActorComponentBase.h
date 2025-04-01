// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TopDownRPG/Player/PlayerSettings.h"
#include "RPGActorComponentBase.generated.h"


UCLASS(Abstract)
class TOPDOWNRPG_API URPGActorComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere, Category = "VFX")
	UPlayerSettings* PlayerSettings;
	
public:
	// Sets default values for this component's properties
	URPGActorComponentBase();

	virtual void SetupComponent(UPlayerSettings* Settings);
	virtual void Dispose() PURE_VIRTUAL()
};
