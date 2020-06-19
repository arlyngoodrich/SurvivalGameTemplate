// Fill out your copyright notice in the Description page of Project Settings.


#include "Interactables/SBasePickup.h"
#include "Character/SCharacter.h"
#include "Components/SInventoryComponent.h"



void ASBasePickup::BeginPlay()
{
	Super::BeginPlay();
}



void ASBasePickup::OnInteract(AActor* InteractingActor)
{
	Super::OnInteract(InteractingActor);

	PerformPickup(InteractingActor);
}


void ASBasePickup::OnFocus()
{
	Super::OnFocus();
	if (bShouldDrawOutline) {ControlOutline(true);}
}



void ASBasePickup::OnLostFocus()
{
	Super::OnLostFocus();
	if (bShouldDrawOutline) { ControlOutline(false); }
}

void ASBasePickup::PerformPickup(AActor* InteractingActor)
{
	if (InteractingActor)
	{
		ASCharacter* InteractingCharacter;
		InteractingCharacter = Cast<ASCharacter>(InteractingActor);

		if (InteractingCharacter)
		{

			USInventoryComponent* InteractingInventoryComponent;
			InteractingInventoryComponent = InteractingCharacter->GetInventoryComponent();

			if (InteractingInventoryComponent->AddItem(InventoryItemData))
			{
				SetLifeSpan(.1f);
			}
		}
	}
}

void ASBasePickup::ControlOutline(bool DrawLine)
{
	
	if (DrawLine)
	{
		BaseStaticMesh->SetRenderCustomDepth(true);
		BaseStaticMesh->SetCustomDepthStencilValue(2);
	}
	else
	{
		BaseStaticMesh->SetRenderCustomDepth(false);
		BaseStaticMesh->SetCustomDepthStencilValue(0);
	}

}