// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHungerComponent.h"
#include "SurvivalGame/SurvivalGame.h"
#include "Character/SCharacter.h"
#include "Components/SStaminaComponent.h"
#include "Components/SHealthComponent.h"

#include "Net/UnrealNetwork.h"
#include "TimerManager.h"



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
	DOREPLIFETIME(USHungerComponent, bIsStarving);


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
	UE_LOG(LogDevelopment, Log, TEXT("HungerCompoennt Initialized"));
	
}

void USHungerComponent::DrainHunger()
{
	DecreaseHunger(HungerDrainAmount);
}

void USHungerComponent::IncreaseHunger(float Amount)
{

	if (GetOwnerRole() > ROLE_Authority)
	{
		Server_IncreaseHunger(Amount);
	}
	else
	{
		CurrentHunger = FMath::Clamp(CurrentHunger + Amount, 0.f, MaxHunger);
	}


}

void USHungerComponent::DecreaseHunger(float Amount)
{

	if (GetOwnerRole() > ROLE_Authority)
	{
		Server_DecreaseHunger(Amount);
	}
	else
	{
		CurrentHunger = FMath::Clamp(CurrentHunger - Amount, 0.f, MaxHunger);
		UE_LOG(LogDevelopment, Log, TEXT("New hunger: %s"), *FString::SanitizeFloat(CurrentHunger));
		

		if (CurrentHunger <= StarvingThreshold)
		{
			StartStarvingDamange();
		}


	}



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

void USHungerComponent::ApplyStarvingDamange()
{
	FDamageEvent Damage;

	Owner->TakeDamage(StarvingDamage, Damage, Owner->GetController(), Owner);
	
	UE_LOG(LogDevelopment, Log, TEXT("Character is starving"))
	
	if (CurrentHunger >= StarvingThreshold)
	{
		EndStarvingDamage();
	}
}

void USHungerComponent::StartStarvingDamange()
{
	// If already starving, no need to call again
	if (GetWorld()->GetTimerManager().TimerExists(StarvingDamangeTimerHandle)) { return; }
	bIsStarving = true;
	GetWorld()->GetTimerManager().SetTimer(StarvingDamangeTimerHandle, this, &USHungerComponent::ApplyStarvingDamange, StarvingDamageInterval, true);

}

void USHungerComponent::EndStarvingDamage()
{
	if (GetWorld()->GetTimerManager().TimerExists(StarvingDamangeTimerHandle))
	{
		bIsStarving = false;
		GetWorld()->GetTimerManager().ClearTimer(StarvingDamangeTimerHandle);
	}

}
