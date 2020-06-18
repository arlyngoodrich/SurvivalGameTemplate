// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SPlayerInteractionComponent.h"
#include "SurvivalGame/SurvivalGame.h"


//Custom Includes
#include "Character/SCharacter.h"
#include "Interactables/SBaseInteractable.h"

//UE4 Includes
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "TimerManager.h"



// Sets default values for this component's properties
USPlayerInteractionComponent::USPlayerInteractionComponent()
{

}


// Called when the game starts
void USPlayerInteractionComponent::BeginPlay()
{
	Super::BeginPlay();

	InitalizeOwnerReferences();
	StartInteractionChecks();
	
}


void USPlayerInteractionComponent::InitalizeOwnerReferences()
{
	if (Cast<ASCharacter>(GetOwner()))
	{
		OwningPlayer = Cast<ASCharacter>(GetOwner());
	}
	else
	{
		UE_LOG(LogDevelopment, Error, TEXT("%s could not find ASCharacter as Owner"), *StaticClass()->GetFName().ToString());
	}
}

void USPlayerInteractionComponent::StartInteractionChecks()
{
	GetWorld()->GetTimerManager().SetTimer(InteractionCheckTimerHandle, this, &USPlayerInteractionComponent::CheckForInteractable, InteractionCheckFrequency, true);
}

void USPlayerInteractionComponent::CheckForInteractable()
{
	FHitResult Hit = PerformTrace();
	ASBaseInteractable* OldInteractable;
	ASBaseInteractable* NewInteractable;

	OldInteractable = InteractableActor;
	NewInteractable = Cast<ASBaseInteractable>(Hit.Actor);

	if (NewInteractable)
	{
		bInteractableInView = true;

		if (OldInteractable)
		{
			if (OldInteractable == NewInteractable)
			{
				//Interactable is the same, don't do anything
				return;
			}
			else
			{
				//Interacable is new, change focus
				NewInteractable->OnFocus();
				OldInteractable->OnLostFocus();
				InteractableActor = NewInteractable;
			}
		}
		else
		{
			//Old Interactable is not valid but tell the new one it has focus
			NewInteractable->OnFocus();
			InteractableActor = NewInteractable;
		}
		
	}
	else
	{
		//No Interactable in view
		bInteractableInView = false;
		if (OldInteractable)
		{
			//Tell old one it has lost focus and set it to null
			OldInteractable->OnLostFocus();
			InteractableActor = nullptr;
		}
	}
	



}

FHitResult USPlayerInteractionComponent::PerformTrace()
{
		
	FVector StartLocation;
	FVector EndLocation;
	FRotator Rotation;
	FHitResult Hit;

	if (OwningPlayer == nullptr) {  return Hit; }

	OwningPlayer->GetController()->GetPlayerViewPoint(StartLocation, Rotation);

	EndLocation = StartLocation + (Rotation.Vector() * InteractableViewDistance);
	
	FCollisionQueryParams TraceParameters;
	TraceParameters.AddIgnoredActor(OwningPlayer);

	bool bHit = GetWorld()->LineTraceSingleByChannel(Hit, StartLocation, EndLocation, ECC_Visibility, TraceParameters);

	if (bShouldShowTrace)
	{
		DrawDebugLine(GetWorld(), StartLocation, EndLocation, FColor::Red, false, InteractionCheckFrequency);
		if (bHit)
		{
			DrawDebugBox(GetWorld(), Hit.ImpactPoint, FVector(5, 5, 5), FColor::Green, false, InteractionCheckFrequency);
		}
	}

	return Hit;
}