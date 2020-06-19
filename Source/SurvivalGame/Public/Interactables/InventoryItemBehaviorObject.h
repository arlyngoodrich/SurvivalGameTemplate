// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "InventoryItemBehaviorObject.generated.h"

class ASCharacter;

UCLASS(Abstract,BlueprintType,Blueprintable,EditInlineNew,DefaultToInstanced)
class SURVIVALGAME_API UInventoryItemBehaviorObject : public UObject
{
	GENERATED_BODY()
	

public:
	
	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void BP_Use(ASCharacter* OwningCharacter);

	virtual void Use(ASCharacter* OwningCharacter);

protected:

	bool bHasBeenInitalize = false;

};
