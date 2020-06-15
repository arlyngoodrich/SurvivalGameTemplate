// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class USHealthComponent;
class USPlayerStateComponent;

UCLASS()
class SURVIVALGAME_API ASCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ASCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// ----- Components -----

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent* CameraComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USpringArmComponent* SpringArmComp;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USHealthComponent* HealthComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USPlayerStateComponent* PlayerStateComponent;


	// ----- Movement Functions -----
	
	void MoveForward(float Value);

	void MoveRight(float Value);

	void BeginCrouch();

	void EndCrouch();

	void StartSprint();

	void EndSprint();

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Player State")
	bool bIsSprinting;

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
	float DefaultRunSpeed = 600;

	UPROPERTY(EditDefaultsOnly, Category = "Movement")
	float DefaultCrouchSpeed = 150;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float YawInput;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void AddControllerYawInput(float Val) override;

};
