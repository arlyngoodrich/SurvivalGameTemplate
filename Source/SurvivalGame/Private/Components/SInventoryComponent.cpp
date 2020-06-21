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

	DOREPLIFETIME(USInventoryComponent, MaxWeight);
	DOREPLIFETIME(USInventoryComponent, CurrentWeight);
	DOREPLIFETIME(USInventoryComponent, Inventory_ItemKeys);
	DOREPLIFETIME(USInventoryComponent, Inventory_ItemNames);
	DOREPLIFETIME(USInventoryComponent, Inventory_ItemData);
	DOREPLIFETIME(USInventoryComponent, LastItemKey);
}


void USInventoryComponent::OnRep_InventoryUpdate()
{
	UE_LOG(LogDevelopment, Log, TEXT("InventoryUpdate"))
	UpdateUI.Broadcast();
}

int32 USInventoryComponent::GetNewKey()
{
	int32 NewItemKey;

	NewItemKey = LastItemKey + 1;

	return NewItemKey;
}

void USInventoryComponent::AddItem(FItemData Item, bool& bItemAdded, int32& ItemID)
{
	if (MaxWeight >= CurrentWeight + Item.ItemWeight)
	{

		if (GetOwnerRole() < ROLE_Authority)
		{
			Server_AddItem(Item);
			bItemAdded = true;
			ItemID = GetNewKey();

		}
		else
		{
			UE_LOG(LogDevelopment, Log, TEXT("Item Added"))
			int32 NewKey = GetNewKey();
			Inventory_ItemKeys.Insert(NewKey,0);
			Inventory_ItemNames.Insert(Item.ItemName, 0);
			Inventory_ItemData.Insert(Item, 0);

			CurrentWeight = CurrentWeight + Item.ItemWeight;

			bItemAdded = true;
			ItemID = NewKey;

			//Update Server
			UpdateUI.Broadcast();
		}


	}
	else
	{
		bItemAdded = false;
	}

}

bool  USInventoryComponent::RemoveItem(int32 ItemKey)
{

	if (Inventory_ItemKeys.Contains(ItemKey))
	{
		if (GetOwnerRole() < ROLE_Authority)
		{
			Server_RemoveItem(ItemKey);
			return true;
		}
		else
		{
			UE_LOG(LogDevelopment, Log, TEXT("Item Removed"))

			int32 Index;
			Index = Inventory_ItemKeys.Find(ItemKey);

			CurrentWeight = CurrentWeight - Inventory_ItemData[Index].ItemWeight;

			Inventory_ItemKeys.RemoveAt(Index);
			Inventory_ItemNames.RemoveAt(Index);
			Inventory_ItemData.RemoveAt(Index);

			//Update Server
			UpdateUI.Broadcast();
		
			return true;

		}
	}
	else
	{

		UE_LOG(LogDevelopment, Log, TEXT("Item key not found"))
		return false;
	}
}

bool USInventoryComponent::Server_AddItem_Validate(FItemData Item)
{
	return true;
}

void USInventoryComponent::Server_AddItem_Implementation(FItemData Item)
{
	bool bItemAdded;
	int32 ItemID;

	AddItem(Item, bItemAdded, ItemID);
}

bool USInventoryComponent::Server_RemoveItem_Validate(int32 ItemID)
{
	return true;
}

void USInventoryComponent::Server_RemoveItem_Implementation(int32 ItemID)
{
	RemoveItem(ItemID);
}


