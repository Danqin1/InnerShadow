// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerStatsComponent.h"
#include "RPGActorComponentBase.h"
#include "Components/ActorComponent.h"
#include "DarknessComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class TOPDOWNRPG_API UDarknessComponent : public URPGActorComponentBase
{
	GENERATED_BODY()
protected:
	bool bIsDark = false;

	float Darkness = 0;
	float MaxDarkness = 100;
	
	UPROPERTY()
	UPlayerStatsComponent* StatsComponent;

	UPROPERTY()
	UPlayerHUD* PlayerHUD;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* DarknessVFXComponent;

	virtual void BeginPlay() override;
	void SetDark(bool bDark);
public:
	UDarknessComponent();
	
	virtual void SetupComponent(UPlayerSettings* Settings) override;
	virtual void Dispose() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateHUD();
	
	void OnDarkness(const FInputActionValue& InputActionValue);
	
	UFUNCTION(BlueprintCallable)
	float GetDarknessPercent() const {return Darkness / MaxDarkness;}

	UFUNCTION(BlueprintCallable)
	bool IsDark() const {return bIsDark;};
	
	UFUNCTION(BlueprintCallable)
	void AddDarkness(float Value);

	UFUNCTION(BlueprintCallable)
	void RemoveDarkness(float Value);

	UFUNCTION(BlueprintCallable)
	float GetDarkness() const {return Darkness;}
};
