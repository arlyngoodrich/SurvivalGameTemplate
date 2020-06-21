// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SGamePlayEffect.generated.h"

class ASCharacter;

/**
 * 
 */
UCLASS(Abstract, BlueprintType, Blueprintable, EditInlineNew, DefaultToInstanced)
class SURVIVALGAME_API USGamePlayEffect : public UObject
{
	GENERATED_BODY()

public:

	USGamePlayEffect();

	//Native Implementation of Trigger, can be overriden by child classes
	virtual void Trigger(ASCharacter* UsingCharacter);

	//Calls Implementatin Event for both Native and Blueprints
	UFUNCTION(BlueprintCallable, Category = "GamePlay Effect", meta = (DisplayName = "Trigger"))
	void BP_Trigger(ASCharacter* UsingCharacter);

	//Blueprint Implementaiton of Triggered 
	UFUNCTION(BlueprintImplementableEvent, Category = "GamePlay Effect", meta = (DisplayName = "On Triggered"))
	void BP_OnTriggered(ASCharacter* UsingCharacter);


};
