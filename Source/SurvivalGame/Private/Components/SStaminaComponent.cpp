// Fill out your copyright notice in the Description page of Project Settings.




#include "Components/SStaminaComponent.h"
#include "SurvivalGame/SurvivalGame.h"

#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"





// Sets default values for this component's properties
USStaminaComponent::USStaminaComponent()
{
	SetIsReplicated(true);
	MaxStamina = 100;
	CurrentStamina = 50;
	StaminaRegenFrequency = 1;


}


// Called when the game starts
void USStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	StartStaminaRegen();
	
}

float USStaminaComponent::GetCurrentStamina() {return CurrentStamina;}

float USStaminaComponent::GetMaxStamina() {return MaxStamina;}


void USStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USStaminaComponent, MaxStamina);
	DOREPLIFETIME(USStaminaComponent, CurrentStamina);
	DOREPLIFETIME(USStaminaComponent, StaminaRegenFrequency);
	
}



void USStaminaComponent::RaiseStamina()
{

	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina++;
		
		//UE_LOG(LogDevelopment, Log, TEXT("Stamina: %s"), *FString::SanitizeFloat(CurrentStamina))
	}

}

void USStaminaComponent::LowerStamina()
{
	
	if (CurrentStamina > 0)
	{
		CurrentStamina--;

		//UE_LOG(LogDevelopment, Log, TEXT("Stamina: %s"), *FString::SanitizeFloat(CurrentStamina))
	}
}

//Automatically starts on the server since called on begin play
void USStaminaComponent::StartStaminaRegen()
{

	if (GetWorld()->GetTimerManager().TimerExists(StaminaRegenTimerHandle) == false)
	{
		//Create Timer
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &USStaminaComponent::RaiseStamina, StaminaRegenFrequency, true);
	}
	else if (GetWorld()->GetTimerManager().IsTimerPaused(StaminaRegenTimerHandle) == true)
	{
		//If Paused, unPause
		GetWorld()->GetTimerManager().UnPauseTimer(StaminaRegenTimerHandle);
	}
	else
	{
		// no idea wtf is happening if it gets here...
		UE_LOG(LogTemp, Warning, TEXT("Unexpected outcome in StartStaminaComponent StartRegenTimer function"));
	}

}

void USStaminaComponent::StopStaminaRegen()
{
	// if timer exists and is not paused
		
	if (GetWorld()->GetTimerManager().TimerExists(StaminaRegenTimerHandle) == true && (GetWorld()->GetTimerManager().IsTimerPaused(StaminaRegenTimerHandle) == false))
	{
		GetWorld()->GetTimerManager().PauseTimer(StaminaRegenTimerHandle);
	}
}



bool USStaminaComponent::Server_OneTimeLowerStamina_Validate(float StaminaToDrain)
{
	return true;
}

void USStaminaComponent::Server_OneTimeLowerStamina_Implementation(float StaminaToDrain)
{

	CurrentStamina = CurrentStamina - StaminaToDrain;

}

bool USStaminaComponent::Server_OneTimeAddStamina_Validate(float StaminaAdd)
{
	return true;
}

void USStaminaComponent::Server_OneTimeAddStamina_Implementation(float StaminaAdd)
{

	RequestOneTimeStaminaAdd(StaminaAdd);

}

bool USStaminaComponent::RequestOneTimeStaminaDrain(float StaminaDrain)
{
	if (CurrentStamina >= StaminaDrain)
	{
		Server_OneTimeLowerStamina(StaminaDrain);
		return true;
	}
	
	
	return false;
}

bool USStaminaComponent::RequestOneTimeStaminaAdd(float StaminaAdd)
{
	UE_LOG(LogDevelopment, Log, TEXT("Requesting Oen Time Stamina Addition Received"));

	if (MaxStamina >= StaminaAdd + CurrentStamina)
	{

		UE_LOG(LogDevelopment, Log, TEXT("Requesting One Time Stamina Addition Approved"));

		if (GetOwnerRole() < ROLE_Authority)
		{
			Server_OneTimeAddStamina(StaminaAdd);
		}
		else
		{
			CurrentStamina = CurrentStamina + StaminaAdd;
			UE_LOG(LogDevelopment, Log, TEXT("Stamina: %s"), *FString::SanitizeFloat(CurrentStamina))
		}

		return true;
	}
	return false;
}



void USStaminaComponent::ControlStaminaRegen(bool StaminaShouldRegen)
{
	

	if (StaminaShouldRegen == true)
	{
		StartStaminaRegen();
	}
	else
	{
		StopStaminaRegen();
	}

}

void USStaminaComponent::RequestStartStaminaDecay(float StaminaDecayRate)
{
	if (GetWorld()->GetTimerManager().TimerExists(StaminaDecayTimerHandle) == true)
	{
		(GetWorld()->GetTimerManager().ClearTimer(StaminaDecayTimerHandle));
	}

	ControlStaminaRegen(false);
	GetWorld()->GetTimerManager().SetTimer(StaminaDecayTimerHandle, this, &USStaminaComponent::LowerStamina, StaminaDecayRate, true);

}

void USStaminaComponent::RequestStopStaminaDecay()
{
	if (GetWorld()->GetTimerManager().TimerExists(StaminaDecayTimerHandle) == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaDecayTimerHandle);
		ControlStaminaRegen(true);
	}

}



