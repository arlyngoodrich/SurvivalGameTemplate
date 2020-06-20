// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Item.generated.h"

class ASCharacter;
/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew)
class SURVIVALGAME_API UItem : public UObject
{
	GENERATED_BODY()

public:
	UItem();

	virtual class UWorld* GetWorld() const { return World; };

	UPROPERTY(Transient)
	class UWorld* World;

protected:

	//Has blueprint Implementation, bool returned by function should be used by calling class to destroy if sucsess 
	virtual bool OnUseItem(ASCharacter* UsingCharacter);

	UFUNCTION(BlueprintCallable, Category = "Interaction", meta = (DisplayName = "UseItem"))
	bool BP_UseItem(ASCharacter* UsingCharacter);

	UPROPERTY(EditDefaultsOnly, Category = "Item Information")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, Category = "Item Information", meta = (MultiLine = true))
	FText Discription;
	
	UPROPERTY(EditDefaultsOnly, Category = "Item Information")
	FText ActionText;

	UPROPERTY(EditDefaultsOnly, Category = "Item Information")
	UTexture2D* Icon;

	UPROPERTY(EditDefaultsOnly, Category = "Item Information")
	float Weight;
};
