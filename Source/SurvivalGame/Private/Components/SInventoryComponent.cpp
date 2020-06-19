// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/SInventoryComponent.h"
#include "SurvivalGame/SurvivalGame.h"
#include "Net/UnrealNetwork.h"



// Sets default values for this component's properties
USInventoryComponent::USInventoryComponent()
{
	SetIsReplicated(true);

}


// Called when the game starts
void USInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


void USInventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(USInventoryComponent, CurrentWeight);
	DOREPLIFETIME(USInventoryComponent, Inventory_ItemIDs);
	DOREPLIFETIME(USInventoryComponent, Inventory_ItemData);


}


bool USInventoryComponent::AddItem(FInventoryData ItemData)
{
	if(ItemData.ItemWeight + CurrentWeight > MaxWeight)
	{
		return false;
	}

	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_AddItem(ItemData);
		return true;
	}
	else
	{

		AddItemData(ItemData);
		UE_LOG(LogDevelopment, Log, TEXT("%s was added to inventory"), *ItemData.DisplayName.ToString())
		return true;
	}
}

bool USInventoryComponent::RemoveItem(FInventoryData ItemData)
{

	if (Inventory_ItemIDs.Find(ItemData.ItemID) == false)
	{
		return false;
	}

	if (GetOwnerRole() < ROLE_Authority)
	{
		Server_RemoveItem(ItemData);
		return true;
	}
	else
	{
		RemoveItemData(ItemData);
		UE_LOG(LogDevelopment, Log, TEXT("%s was removed from inventory"), *ItemData.DisplayName.ToString())
		return true;
	}
}

bool USInventoryComponent::BP_AddItem(FInventoryData ItemData)
{
	return AddItem(ItemData);
}

bool USInventoryComponent::BP_RemoveItem(FInventoryData ItemData)
{
	return RemoveItem(ItemData);
}


bool USInventoryComponent::Server_AddItem_Validate(FInventoryData ItemData)
{
	return true;
}

void USInventoryComponent::Server_AddItem_Implementation(FInventoryData ItemData)
{
	AddItem(ItemData);
}

bool USInventoryComponent::Server_RemoveItem_Validate(FInventoryData ItemData)
{
	return true;
}

void USInventoryComponent::Server_RemoveItem_Implementation(FInventoryData ItemData)
{
	RemoveItem(ItemData);
}

void USInventoryComponent::AddItemData(FInventoryData ItemData)
{

	float OldWeight = CurrentWeight;

	CurrentWeight = OldWeight + ItemData.ItemWeight;
	Inventory_ItemIDs.Add(ItemData.ItemID);
	Inventory_ItemData.Add(ItemData);

}

void USInventoryComponent::RemoveItemData(FInventoryData ItemData)
{
	int32 ItemIndex;
	Inventory_ItemIDs.Find(ItemData.ItemID, ItemIndex);
	Inventory_ItemIDs.Remove(ItemData.ItemID);
	Inventory_ItemData.RemoveAtSwap(ItemIndex, 1, true);

}
