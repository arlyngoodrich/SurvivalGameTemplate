// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHungerComponent.generated.h"

//////////// ===== Please Note ====== \\\\\\\\\\\\
// This component must be owned by a character with a stamina and health component.    

class USStaminaComponent;
class USHealthComponent;
class ASCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API USHungerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHungerComponent();



protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Initalize();

	UFUNCTION()
	void DrainHunger();

public:

	UFUNCTION(BlueprintCallable, Category = "Hunger")
	void IncreaseHunger(float Amount);

	UFUNCTION(BlueprintCallable, Category = "Hunger")
	void DecreaseHunger(float Amount);

protected:


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_IncreaseHunger(float Amount);
	bool Server_IncreaseHunger_Validate(float Amount);
	void Server_IncreaseHunger_Implementation(float Amount);


	UFUNCTION(Server, Reliable, WithValidation)
	void Server_DecreaseHunger(float Amount);
	bool Server_DecreaseHunger_Validate(float Amount);
	void Server_DecreaseHunger_Implementation(float Amount);

	ASCharacter* Owner;
	USStaminaComponent* OwnerStaminaComponent;

	FTimerHandle StarvingDamangeTimerHandle;

	void ApplyStarvingDamange();
	
	void StartStarvingDamange();

	void EndStarvingDamage();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Hugner")
	bool bIsStarving;


	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Hugner")
	float MaxHunger = 100.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hugner")
	float StarvingThreshold = 20.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hugner")
	float StarvingDamage = 10.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hugner")
	float StarvingDamageInterval = 1.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Hugner")
	float HungerDrainAmount = 1.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Hunger")
	float CurrentHunger;

};
