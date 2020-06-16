// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "StaminaComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API UStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	UFUNCTION(BlueprintPure, Category = "Player State")
	float GetCurrentStamina();

protected:

	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stamina")
	float MaxStamina;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Stamina")
	float CurrentStamina;
	
	UPROPERTY(Replicated, EditDefaultsOnly, Category = "Stamina")
	float StaminaRegenFrequency;

	FTimerHandle StaminaRegenTimerHandle;

	FTimerHandle StaminaDecayTimerHandle;

	
	void RaiseStamina();

	void LowerStamina();

	void StartStaminaRegen();

	void StopStaminaRegen();

public:

	void ControlStaminaRegen(bool StaminaShouldRegen);

	void StartStaminaDecay(float StaminaDecayRate);

	void StopStaminaDecay();

};
