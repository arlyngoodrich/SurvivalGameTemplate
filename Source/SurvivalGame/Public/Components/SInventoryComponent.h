// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactables/SInventoryData.h"
#include "SInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FUpdateUI);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API USInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInventoryComponent();

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	void AddItem(FItemData Item, bool& bItemAdded, int32& ItemID);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool RemoveItem(int32 ItemID);

	UPROPERTY(BlueprintAssignable)
	FUpdateUI UpdateUI;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadOnly, Category = "Inventory")
	float MaxWeight = 200.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	float CurrentWeight;


	UPROPERTY(ReplicatedUsing = OnRep_InventoryUpdate, BlueprintReadOnly, Category = "Inventory")
	TArray<int32> Inventory_ItemKeys;

	//Unique Item ID, ItemName
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	TArray<FName> Inventory_ItemNames;

	//Unique Item ID, Item Data
	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	TArray<FItemData> Inventory_ItemData;

	UFUNCTION()
	void OnRep_InventoryUpdate();

	int32 GetNewKey();

	UPROPERTY(Replicated)
	int32 LastItemKey;

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddItem(FItemData Item);
	bool Server_AddItem_Validate(FItemData Item);
	void Server_AddItem_Implementation(FItemData Item);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(int32 ItemID);
	bool Server_RemoveItem_Validate(int32 ItemID);
	void Server_RemoveItem_Implementation(int32 ItemID);

};
