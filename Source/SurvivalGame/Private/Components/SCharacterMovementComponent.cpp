// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SCharacterMovementComponent.h"
#include "SCharacter.h"



void USCharacterMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwningCharacter = Cast<ASCharacter>(PawnOwner);
	if (OwningCharacter == nullptr) { UE_LOG(LogTemp, Warning, TEXT("SCharacterMovementComponent could not find Owning Character")) return; }


	DefaultMaxSpeed = OwningCharacter->GetDefaultWalkSpeed();

}


float USCharacterMovementComponent::GetMaxSpeed() const
{
	float MaxSpeed = Super::GetMaxSpeed();
	
	if (OwningCharacter)
	{
		if (OwningCharacter->GetWantsToSprint())
		{
			MaxSpeed = DefaultMaxSpeed * OwningCharacter->GetSprintSpeedModifier();
		}
		else
		{
			MaxSpeed = DefaultMaxSpeed;
		}
	}

	return MaxSpeed;
}