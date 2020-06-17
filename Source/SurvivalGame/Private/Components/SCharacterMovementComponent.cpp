// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalGame/SurvivalGame.h"

#include "Components/SCharacterMovementComponent.h"
#include "SCharacter.h"



void USCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ASCharacter>(PawnOwner);
	if (OwningCharacter == nullptr) { UE_LOG(LogDevelopment, Error, TEXT("SCharacterMovementComponent could not find Owning Character")) return; }


	DefaultMaxSpeed = OwningCharacter->GetDefaultWalkSpeed();

}


float USCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	
	if (OwningCharacter)
	{
		if (OwningCharacter->GetWantsToSprint() && !IsCrouching())
		{
			MaxSpeed = DefaultMaxSpeed * OwningCharacter->GetSprintSpeedModifier();
			OwningCharacter->SetIsSprinting(true);
		}
		else
		{
			MaxSpeed = DefaultMaxSpeed;

			if (OwningCharacter->GetWantsToSprint() == true)
			{
				OwningCharacter->InteruptSprint();
				OwningCharacter->SetIsSprinting(false);
			}
		}
	}

	return MaxSpeed;
}