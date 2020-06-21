// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHungerComponent.h"
#include "SurvivalGame/SurvivalGame.h"
#include "Character/SCharacter.h"
#include "Components/SStaminaComponent.h"
#include "Components/SHealthComponent.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USHungerComponent::USHungerComponent()
{
	
	SetIsReplicated(true);
	CurrentHunger = MaxHunger;
	
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features

}


void USHungerComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);


	DOREPLIFETIME(USHungerComponent, CurrentHunger);
	DOREPLIFETIME(USHungerComponent, MaxHunger);


}


// Called when the game starts
void USHungerComponent::BeginPlay()
{
	Super::BeginPlay();

	Initalize();
	
}

void USHungerComponent::Initalize()
{
	Owner = Cast<ASCharacter>(GetOwner());
	if (Owner == nullptr) { UE_LOG(LogDevelopment, Error, TEXT("Stamina component could not find character owner")) return; }

	OwnerStaminaComponent = Owner->GetStaminaComponent();
	OwnerStaminaComponent->OnStaminaRegen.AddDynamic(this, &USHungerComponent::DrainHunger);
	
}

void USHungerComponent::DrainHunger()
{
	DecreaseHunger(HungerDrainAmount);
}

void USHungerComponent::IncreaseHunger(float Amount)
{
}

void USHungerComponent::DecreaseHunger(float Amount)
{
}


bool USHungerComponent::Server_IncreaseHunger_Validate(float Amount)
{
	return true;
}

void USHungerComponent::Server_IncreaseHunger_Implementation(float Amount)
{
	IncreaseHunger(Amount);
}



bool USHungerComponent::Server_DecreaseHunger_Validate(float Amount)
{
	return true;
}

void USHungerComponent::Server_DecreaseHunger_Implementation(float Amount)
{
	DecreaseHunger(Amount);
}
