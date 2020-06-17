// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SBaseInteractable.h"

// Sets default values
ASBaseInteractable::ASBaseInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASBaseInteractable::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASBaseInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

