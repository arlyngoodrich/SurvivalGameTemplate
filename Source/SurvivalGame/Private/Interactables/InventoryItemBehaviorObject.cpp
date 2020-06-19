// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/InventoryItemBehaviorObject.h"
#include "Character/SCharacter.h"
#include "SurvivalGame/SurvivalGame.h"



void UInventoryItemBehaviorObject::BP_Use(ASCharacter* OwningCharacter)
{
	Use(OwningCharacter);
}

void UInventoryItemBehaviorObject::Use(ASCharacter* OwningCharacter)
{
	UE_LOG(LogDevelopment, Log, TEXT("%s interacted with"), *StaticClass()->GetFName().ToString());
}
