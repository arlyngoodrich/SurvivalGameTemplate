// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SCharacterMovementComponent.generated.h"

/**
 * 
 */


class ASCharacter;

UCLASS()
class SURVIVALGAME_API USCharacterMovementComponent : public UCharacterMovementComponent
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

	virtual float GetMaxSpeed() const override;

protected:

	ASCharacter* OwningCharacter;

	float DefaultMaxSpeed;
	
};
