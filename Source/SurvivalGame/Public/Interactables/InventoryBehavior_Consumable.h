// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/InventoryItemBehaviorObject.h"
#include "InventoryBehavior_Consumable.generated.h"

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UInventoryBehavior_Consumable : public UInventoryItemBehaviorObject
{
	GENERATED_BODY()
	
public:

	virtual void Use(ASCharacter* OwningCharacter) override;

	//returns true if should be removed
	UFUNCTION(BlueprintCallable, Category = "Consumable")
	bool Consume(ASCharacter* OwningCharacter);

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	float DeltaStamina;
};
