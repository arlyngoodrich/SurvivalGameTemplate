// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SBaseInteractable.h"
#include "SurvivalGame/SurvivalGame.h"

// UE4 Includes
#include "Components/StaticMeshComponent.h"

// Sets default values
ASBaseInteractable::ASBaseInteractable()
{
	BaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseStaticMesh"));
}

// Called when the game starts or when spawned
void ASBaseInteractable::BeginPlay()
{
	Super::BeginPlay();
}

void ASBaseInteractable::OnInteract()
{
	UE_LOG(LogDevelopment, Log, TEXT("%s interacted with"), *StaticClass()->GetFName().ToString());
}

void ASBaseInteractable::OnFocus()
{
	UE_LOG(LogDevelopment, Log, TEXT("%s has focus"), *StaticClass()->GetFName().ToString());
}

void ASBaseInteractable::OnLostFocus()
{
	UE_LOG(LogDevelopment, Log, TEXT("%s has focus"), *StaticClass()->GetFName().ToString());
}



