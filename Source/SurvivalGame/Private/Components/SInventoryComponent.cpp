// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SInventoryComponent.h"
#include "SurvivalGame/SurvivalGame.h"

#include "Net/UnrealNetwork.h"



// Sets default values for this component's properties
USInventoryComponent::USInventoryComponent()
{
	SetIsReplicated(true);

}


// Called when the game starts
void USInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void USInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USInventoryComponent, MaxWeight);
	DOREPLIFETIME(USInventoryComponent, CurrentWeight);
	
}



