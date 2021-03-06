// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SStaminaComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaRegen);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStaminaDrain);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API USStaminaComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USStaminaComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	
	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetCurrentStamina();

	UFUNCTION(BlueprintPure, Category = "Stamina")
	float GetMaxStamina();

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaRegen OnStaminaRegen;

	UPROPERTY(BlueprintAssignable, Category = "Stamina")
	FOnStaminaRegen OnStaminaDrain;

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

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OneTimeLowerStamina(float StaminaToDrain);
	bool Server_OneTimeLowerStamina_Validate(float StaminaToDrain);
	void Server_OneTimeLowerStamina_Implementation(float StaminaToDrain);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_OneTimeAddStamina(float StaminaAdd);
	bool Server_OneTimeAddStamina_Validate(float StaminaAdd);
	void Server_OneTimeAddStamina_Implementation(float StaminaAdd);

public:

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool RequestOneTimeStaminaDrain(float StaminaDrain);

	UFUNCTION(BlueprintCallable, Category = "Stamina")
	bool RequestOneTimeStaminaAdd(float StaminaAdd);

	void ControlStaminaRegen(bool StaminaShouldRegen);

	void RequestStartStaminaDecay(float StaminaDecayRate);

	void RequestStopStaminaDecay();

};
