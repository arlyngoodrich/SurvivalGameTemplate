// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPlayerStateComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API USPlayerStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USPlayerStateComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void StartTimers();

	void DecayHungerAndThirst();

	void RegenStamina();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LowerHunger(float Value);
	bool Server_LowerHunger_Validate(float Value);
	void Server_LowerHunger_Implementation(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LowerThirst(float Value);
	bool Server_LowerThirst_Validate(float Value);
	void Server_LowerThirst_Implementation(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_LowerStamina(float Value);
	bool Server_LowerStamina_Validate(float Value);
	void Server_LowerStamina_Implementation(float Value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_ControlStaminaRegenTimer(bool ShouldTimerRun);
	bool Server_ControlStaminaRegenTimer_Validate(bool ShouldTimerRun);
	void Server_ControlStaminaRegenTimer_Implementation(bool ShouldTimerRun);

	FTimerHandle HungerAndThirstDecayTimerHandle;

	FTimerHandle StaminaDecayTimerHandle;

	FTimerHandle StaminaRegenTimerHandle;


	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
	float Hunger;

	UPROPERTY(EditDefaultsOnly, Category = "Player State")
	float HungerDecrementValue;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
	float Thirst;

	UPROPERTY(EditDefaultsOnly, Category = "Player State")
	float ThirstDecrementValue;

	UPROPERTY(EditDefaultsOnly, Category = "Player State")
	float HungerAndThirstDecayInterval;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
	float Stamina;

	UPROPERTY(EditDefaultsOnly, Category = "Player State")
	float SprintStaminaDecrementInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Player State")
	float SprintStaminaDecrementValue;

	UPROPERTY(EditDefaultsOnly, Category = "Player State")
	float StaminaRegenInterval;

	UPROPERTY(EditDefaultsOnly, Category = "Player State")
	float StaminaRegenValue;


public:	

	void LowerHunger(float value);

	void LowerThirst(float value);

	void LowerStamina(float value);

	void ControlStaminaRegenTimer(bool ShouldTimerRun);

	UFUNCTION(BlueprintPure,Category = "Player State")
	float GetHunger();
	
	UFUNCTION(BlueprintPure,Category = "Player State")
	float GetThirst();

	UFUNCTION(BlueprintPure,Category = "Player State")
	float GetStamina();

	UFUNCTION(BlueprintPure, Category = "Player State")
	float GetSprintStaminaDecrementInterval();

	UFUNCTION(BlueprintPure, Category = "Player State")
	float GetSprintStaminaDecrementValue();

		
};
