// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interactables/BaseClasses/Item.h"
#include "Item_StaminaBoost.generated.h"

class ASCharacter;

/**
 * 
 */
UCLASS()
class SURVIVALGAME_API UItem_StaminaBoost : public UItem
{
	GENERATED_BODY()

public:
	UItem_StaminaBoost();

protected:

	virtual bool OnUseItem(ASCharacter* UsingCharacter) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Information")
	float StaminaBoostAmount;
};
