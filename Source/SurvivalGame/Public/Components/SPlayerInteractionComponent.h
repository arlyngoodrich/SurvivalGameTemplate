// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SPlayerInteractionComponent.generated.h"

class ASCharacter;
class UCameraComponent;
class ASBaseInteractable;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API USPlayerInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USPlayerInteractionComponent();

	void Interact();

	bool GetIsInteractableInView();

	ASBaseInteractable* GetInteractableInView();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	ASCharacter* OwningPlayer;

	UCameraComponent* PlayerCamera;

	void InitalizeOwnerReferences();

	void StartInteractionChecks();

	void CheckForInteractable();

	FHitResult PerformTrace();

	FTimerHandle InteractionCheckTimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	float InteractableViewDistance = 2000.f;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	float InteractionCheckFrequency = 0.1f;

	UPROPERTY(EditDefaultsOnly, Category = "Configuration")
	bool bShouldShowTrace = true;

	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
	bool bInteractableInView;

	//Check if Null before using! 
	UPROPERTY(BlueprintReadOnly, Category = "Interactable")
	ASBaseInteractable* InteractableActor;
};
