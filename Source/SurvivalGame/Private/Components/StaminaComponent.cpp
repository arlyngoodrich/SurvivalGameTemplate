// Fill out your copyright notice in the Description page of Project Settings.


#include "Engine/Engine.h"
#include "Net/UnrealNetwork.h"
#include "TimerManager.h"

#include "Components/StaminaComponent.h"



// Sets default values for this component's properties
UStaminaComponent::UStaminaComponent()
{
	SetIsReplicated(true);
	MaxStamina = 100;
	CurrentStamina = 50;
	StaminaRegenFrequency = 1;


}


// Called when the game starts
void UStaminaComponent::BeginPlay()
{
	Super::BeginPlay();

	StartStaminaRegen();
	
}

float UStaminaComponent::GetCurrentStamina() {return CurrentStamina;}


void UStaminaComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(UStaminaComponent, MaxStamina);
	DOREPLIFETIME(UStaminaComponent, CurrentStamina);
	DOREPLIFETIME(UStaminaComponent, StaminaRegenFrequency);
	
}



void UStaminaComponent::RaiseStamina()
{

	if (CurrentStamina < MaxStamina)
	{
		CurrentStamina++;
		
		//UE_LOG(LogTemp, Warning, TEXT("Stamina: %s"), *FString::SanitizeFloat(CurrentStamina))
	}

}

void UStaminaComponent::LowerStamina()
{
	
	if (CurrentStamina > 0)
	{
		CurrentStamina--;

		//UE_LOG(LogTemp, Warning, TEXT("Stamina: %s"), *FString::SanitizeFloat(CurrentStamina))
	}

}

//Automatically starts on the server since called on begin play
void UStaminaComponent::StartStaminaRegen()
{

	if (GetWorld()->GetTimerManager().TimerExists(StaminaRegenTimerHandle) == false)
	{
		//Create Timer
		GetWorld()->GetTimerManager().SetTimer(StaminaRegenTimerHandle, this, &UStaminaComponent::RaiseStamina, StaminaRegenFrequency, true);
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

void UStaminaComponent::StopStaminaRegen()
{
	// if timer exists and is not paused
		
	if (GetWorld()->GetTimerManager().TimerExists(StaminaRegenTimerHandle) == true && (GetWorld()->GetTimerManager().IsTimerPaused(StaminaRegenTimerHandle) == false))
	{
		GetWorld()->GetTimerManager().PauseTimer(StaminaRegenTimerHandle);
	}
}



bool UStaminaComponent::Server_OneTimeLowerStamina_Validate(float StaminaToDrain)
{
	return true;
}

void UStaminaComponent::Server_OneTimeLowerStamina_Implementation(float StaminaToDrain)
{

	CurrentStamina = CurrentStamina - StaminaToDrain;

}

bool UStaminaComponent::RequestOneTimeStaminaDrain(float StaminaDrain)
{
	if (CurrentStamina >= StaminaDrain)
	{
		Server_OneTimeLowerStamina(StaminaDrain);
		return true;
	}
	
	
	return false;
}



void UStaminaComponent::ControlStaminaRegen(bool StaminaShouldRegen)
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

void UStaminaComponent::RequestStartStaminaDecay(float StaminaDecayRate)
{
	if (GetWorld()->GetTimerManager().TimerExists(StaminaDecayTimerHandle) == false)
	{
		GetWorld()->GetTimerManager().SetTimer(StaminaDecayTimerHandle, this, &UStaminaComponent::LowerStamina, StaminaDecayRate, true);
	}
}

void UStaminaComponent::RequestStopStaminaDecay()
{
	if (GetWorld()->GetTimerManager().TimerExists(StaminaDecayTimerHandle) == true)
	{
		GetWorld()->GetTimerManager().ClearTimer(StaminaDecayTimerHandle);
	}

}



