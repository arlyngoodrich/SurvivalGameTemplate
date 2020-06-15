// Fill out your copyright notice in the Description page of Project Settings.


#include "SHealthComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	
	SetIsReplicated(true);
	DefaultHealth = 100;
	Health = DefaultHealth;


}


// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}


