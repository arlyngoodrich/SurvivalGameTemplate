// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/BaseClasses/SBaseInteractable.h"
#include "SBasePickup.generated.h"

class UItem;

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API ASBasePickup : public ASBaseInteractable
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;

	virtual void OnInteract(AActor* InteractingActor) override;

	virtual void OnFocus() override;

	virtual void OnLostFocus() override;

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bShouldDrawOutline;

	void PerformPickup(AActor* InteractingActor);

	void ControlOutline(bool DrawLine);


};
