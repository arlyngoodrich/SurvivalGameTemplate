// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SHealthComponent.h"
#include "SurvivalGame/SurvivalGame.h"
#include "Character/SCharacter.h"

#include "Net/UnrealNetwork.h"


// Sets default values for this component's properties
USHealthComponent::USHealthComponent()
{
	
	SetIsReplicated(true);
	MaxHealth = 100;
	CurrentHealth = MaxHealth;
}



// Called when the game starts
void USHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	Initalize();

}

void USHealthComponent::Initalize()
{
	AActor* MyOwner = GetOwner();
	if (MyOwner)
	{
		UE_LOG(LogDevelopment, Log, TEXT("Health Component intialized"));
		MyOwner->OnTakeAnyDamage.AddDynamic(this, &USHealthComponent::OnOwnerTakeDamange);
	}
}


void USHealthComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USHealthComponent, CurrentHealth);
}


void USHealthComponent::OnOwnerTakeDamange(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	UE_LOG(LogDevelopment, Log, TEXT("%s damage received"), *FString::SanitizeFloat(Damage));
	
	// If its 0 or less, nobody cares
	if (Damage <= 0.0f) { return; }
	
	CurrentHealth = FMath::Clamp(CurrentHealth - Damage, 0.f, MaxHealth);

	UE_LOG(LogDevelopment, Log, TEXT("New health is %s"), *FString::SanitizeFloat(CurrentHealth));

	if (CurrentHealth <= 0)
	{
		OnDeath.Broadcast();
	}

}

void USHealthComponent::OnRep_HealthChanged()
{
	OnHealthChange.Broadcast(CurrentHealth);
}

void USHealthComponent::IncreaseHealth(float Amount)
{
	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_IncreaseHealth(Amount);
	}
	else
	{
		CurrentHealth = FMath::Clamp(CurrentHealth + Amount, 0.f, MaxHealth);
		UE_LOG(LogDevelopment, Log, TEXT("New health is %s"), *FString::SanitizeFloat(CurrentHealth));
	}
}

bool USHealthComponent::Server_IncreaseHealth_Validate(float Amount)
{
	return true;
}

void USHealthComponent::Server_IncreaseHealth_Implementation(float Amount)
{
	IncreaseHealth(Amount);
}
