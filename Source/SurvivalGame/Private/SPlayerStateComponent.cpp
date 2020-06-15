// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerStateComponent.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"
#include "Engine/Engine.h"

// Sets default values for this component's properties
USPlayerStateComponent::USPlayerStateComponent()
{

	SetIsReplicated(true);
	Hunger = 100.f;
	Thirst = 100.f;
	Stamina = 50.f;

	HungerAndThirstDecayInterval = 3.f;
	StaminaRegenInterval = 1.f;
	SprintStaminaDecrementInterval = 0.25f;

	StaminaRegenValue = 1.f;
	SprintStaminaDecrementValue = 2.f;

	ThirstDecrementValue = 0.5f;
	HungerDecrementValue = 0.3f;

}


// Called when the game starts
void USPlayerStateComponent::BeginPlay()
{
	Super::BeginPlay();

	StartTimers();
	
}

void USPlayerStateComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USPlayerStateComponent, Thirst);
	DOREPLIFETIME(USPlayerStateComponent, Hunger);
	DOREPLIFETIME(USPlayerStateComponent, Stamina);

}


float USPlayerStateComponent::GetHunger() { return Hunger; }
float USPlayerStateComponent::GetThirst() { return Thirst; }
float USPlayerStateComponent::GetStamina() { return Stamina; }
float USPlayerStateComponent::GetSprintStaminaDecrementInterval() { return SprintStaminaDecrementInterval; }
float USPlayerStateComponent::GetSprintStaminaDecrementValue() { return SprintStaminaDecrementValue; }



void USPlayerStateComponent::StartTimers()
{
	// Hunger and Thirst Timer
		GetWorld()->GetTimerManager().SetTimer(HungerAndThirstDecayTimerHandle, this, &USPlayerStateComponent::DecayHungerAndThirst, HungerAndThirstDecayInterval, true);

	// Stamina Regen Timer
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &USPlayerStateComponent::RegenStamina, StaminaRegenInterval, true);
}

void USPlayerStateComponent::DecayHungerAndThirst()
{

	if (GetOwnerRole() == ROLE_Authority)
	{
		LowerHunger(HungerDecrementValue);
		LowerThirst(ThirstDecrementValue);
	}
}

void USPlayerStateComponent::LowerHunger(float Value)
{

	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_LowerHunger(Value);

	}
	else
	{
		Hunger = Hunger - Value;
	}
}

void USPlayerStateComponent::LowerThirst(float Value)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_LowerThirst(Value);
	}
	else
	{
		Thirst = Thirst - Value;
	}

}

void USPlayerStateComponent::LowerStamina(float Value)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_LowerStamina(Value);
	}
	else
	{
		Stamina = Stamina - Value;
	}

}




bool USPlayerStateComponent::Server_LowerHunger_Validate(float Value)
{
	return true;
}

void USPlayerStateComponent::Server_LowerHunger_Implementation(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		LowerHunger(Value);
	}

}

bool USPlayerStateComponent::Server_LowerThirst_Validate(float Value)
{
	return true;
}

void USPlayerStateComponent::Server_LowerThirst_Implementation(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		LowerThirst(Value);
	}
}

bool USPlayerStateComponent::Server_LowerStamina_Validate(float Value)
{
	return true;
}

void USPlayerStateComponent::Server_LowerStamina_Implementation(float Value)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		LowerStamina(Value);
	}
}


void USPlayerStateComponent::RegenStamina()
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		if (Stamina > 100)
		{
			Stamina = 100;
		}
		else
		{
			Stamina = Stamina + StaminaRegenValue;
		}
	}
}


void USPlayerStateComponent::ControlStaminaRegenTimer(bool ShouldTimerRun)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_ControlStaminaRegenTimer(ShouldTimerRun);
		return;
	}
	else
	{
		if (ShouldTimerRun == false)
		{
			GetWorld()->GetTimerManager().PauseTimer(StaminaRegenTimerHandle);
		}
		else if (ShouldTimerRun == true && GetWorld()->GetTimerManager().IsTimerPaused(StaminaRegenTimerHandle) == true)
		{
			GetWorld()->GetTimerManager().UnPauseTimer(StaminaRegenTimerHandle);
		}
	}
}

bool USPlayerStateComponent::Server_ControlStaminaRegenTimer_Validate(bool ShouldTimerRun)
{
	return true;
}

void USPlayerStateComponent::Server_ControlStaminaRegenTimer_Implementation(bool ShouldTimerRun)
{
	if (GetOwnerRole() == ROLE_Authority)
	{
		ControlStaminaRegenTimer(ShouldTimerRun);
	}

}