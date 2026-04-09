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
	float DefaultMaxWalkSpeed = 0;
	
	UPROPERTY()
	UPlayerStatsComponent* StatsComponent;

	UPROPERTY()
	UPlayerHUD* PlayerHUD;

	UPROPERTY()
	class UCharacterMovementComponent* CharacterMovement;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UNiagaraComponent* RageVFXComponent;

	FTimerHandle FrenzyRecoveryTimer;

	virtual void BeginPlay() override;
	void StartFrenzy();
	void SetInRage(bool bDark);
	void FinishFrenzy();
	void RecoverFromFrenzy();
public:
	URageComponent();
	
	virtual void SetupComponent(UPlayerSettings* Settings) override;
	virtual void Dispose() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;

	void UpdateHUD();
	
	UFUNCTION(BlueprintCallable)
	float GetRagePercent() const;

	UFUNCTION(BlueprintCallable)
	bool IsInRage() const {return bIsInRage;};
	
	UFUNCTION(BlueprintCallable)
	void AddRage(float Value);
};
