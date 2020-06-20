// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/Items/Item_StaminaBoost.h"
#include "Character/SCharacter.h"
#include "Components/SStaminaComponent.h"
#include "SurvivalGame/SurvivalGame.h"

UItem_StaminaBoost::UItem_StaminaBoost()
{
	StaminaBoostAmount = 5;
}

bool UItem_StaminaBoost::OnUseItem(ASCharacter* UsingCharacter)
{
	Super::OnUseItem(UsingCharacter);

	UE_LOG(LogDevelopment, Log, TEXT("StaminaBoost Item Called"));
	UE_LOG(LogDevelopment, Log, TEXT("Stamina Boos Amout: %s"), *FString::SanitizeFloat(StaminaBoostAmount))


	if (UsingCharacter)
	{
		return UsingCharacter->GetStaminaComponent()->RequestOneTimeStaminaAdd(StaminaBoostAmount);
	}
	else
	{
		UE_LOG(LogDevelopment, Warning, TEXT("StaminaBoost Item did not receive character reference"));
		return false;
	}
}
