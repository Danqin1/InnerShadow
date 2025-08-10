// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStatsComponent.h"
#include "RPGActorComponentBase.h"
#include "Components/ActorComponent.h"
#include "RageComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API URageComponent : public URPGActorComponentBase
{
	GENERATED_BODY()
protected:
	bool bIsInRage = false;

	float Rage = 0;
	float MaxRage = 100;
	
	UPROPERTY()
	UPlayerStatsComponent* StatsComponent;

	UPROPERTY()
	UPlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* RageVFXComponent;

	virtual void BeginPlay() override;
	void SetInRage(bool bDark);
public:
	URageComponent();
	
	virtual void SetupComponent(UPlayerSettings* Settings) override;
	virtual void Dispose() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateHUD();
	
	void OnRage(const FInputActionValue& InputActionValue);
	
	UFUNCTION(BlueprintCallable)
	float GetRagePercent() const {return Rage / MaxRage;}

	UFUNCTION(BlueprintCallable)
	bool IsInRage() const {return bIsInRage;};
	
	UFUNCTION(BlueprintCallable)
	void AddRage(float Value);
};
