// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USHealthComponent;
class USStaminaComponent;
class USPlayerInteractionComponent;
class ASBaseInteractable;
class USInventoryComponent;
class UItem;
class USHungerComponent;

UCLASS()
class SURVIVALGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ASCharacter(const class FObjectInitializer& ObjectInitializer);
	


	// ======= Public Getters/Setters ======= ======= ======= =======
	
	bool GetWantsToSprint();

	void SetIsSprinting(bool IsSprinting);

	float GetSprintSpeedModifier();

	float GetDefaultWalkSpeed();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	USInventoryComponent* GetInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "State")
	USStaminaComponent* GetStaminaComponent();

	UFUNCTION(BlueprintPure, Category = "Player State")
	float GetMinStaminaToSprint();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ======= Components ======= ======= ======= =======

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USStaminaComponent* StaminaComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USPlayerInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USInventoryComponent* InventoryComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHungerComponent* HungerComponent;


	// =======  Movement Functions ======= ======= ======= =======

public:
	// ----- Public Movement Functions -----

	void InteruptSprint();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerYawInput(float Val) override;


protected:
	// ----- Protected Movement Functions -----
	
	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void StartSprint();

	void EndSprint();

	virtual void Jump() override;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
	bool bWantsToSprint;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
	bool bIsSprinting;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
	bool bIsAlive = true;

	//Only called from server health component
	UFUNCTION()
	void OnDeath();

	UFUNCTION(Client, Reliable)
	void Client_OnDeath();
	void Client_OnDeath_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_StartSprinting();
	bool Server_StartSprinting_Validate();
	void Server_StartSprinting_Implementation();

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_EndSprinting();
	bool Server_EndSprinting_Validate();
	void Server_EndSprinting_Implementation();


	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DefaultWalkSpeed = 175;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintSpeedMuliplyer = 3;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DefaultCrouchSpeed = 150;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float MinStamToSprint = 10;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float SprintStamDecayRate = .1;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float StaminaRequiredToJump = 10;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float YawInput;

	// ======== Player Interaction ======= ======= ======= =======


	// ----- Public Interaction Functions -----

	
	// ----- Protected Interaction Functions -----

	void TriggerInteract();

	void Interact(ASBaseInteractable* Interactable);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Interact(ASBaseInteractable* Interactable);
	bool Server_Interact_Validate(ASBaseInteractable* Interactable);
	void Server_Interact_Implementation(ASBaseInteractable* Interactable);

};
