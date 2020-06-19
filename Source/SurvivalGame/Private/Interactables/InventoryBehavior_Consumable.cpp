// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/InventoryBehavior_Consumable.h"
#include "Character/SCharacter.h"
#include "Components/SStaminaComponent.h"


void UInventoryBehavior_Consumable::Use(ASCharacter* OwningCharacter)
{
	Super::Use(OwningCharacter);

}

bool UInventoryBehavior_Consumable::Consume(ASCharacter* OwningCharacter)
{

	USStaminaComponent* OwningStaminaComponent;
	OwningStaminaComponent = OwningCharacter->GetStaminaComponent();

	if (OwningStaminaComponent->RequestOneTimeStaminaAdd(DeltaStamina))
	{
		return true;
	}
	else
	{
		return false;
	}
	
}
