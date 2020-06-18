// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SBaseInteractable.h"
#include "SurvivalGame/SurvivalGame.h"

// UE4 Includes
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASBaseInteractable::ASBaseInteractable()
{
	BaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseStaticMesh"));
	SetRootComponent(BaseStaticMesh);
	SetReplicates(true);
}

// Called when the game starts or when spawned
void ASBaseInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void ASBaseInteractable::OnInteract(AActor* InteractingActor)
{
	UE_LOG(LogDevelopment, Log, TEXT("%s interacted with"), *StaticClass()->GetFName().ToString());
	
	if (InteractingActor)
	{
		BP_OnInteracted(InteractingActor);
	}
}


void ASBaseInteractable::OnFocus()
{
	UE_LOG(LogDevelopment, Log, TEXT("%s has focus"), *StaticClass()->GetFName().ToString());

	BP_OnFocus();

}

void ASBaseInteractable::OnLostFocus()
{
	UE_LOG(LogDevelopment, Log, TEXT("%s has lost focus"), *StaticClass()->GetFName().ToString());

	BP_OnLostFocus();
}




