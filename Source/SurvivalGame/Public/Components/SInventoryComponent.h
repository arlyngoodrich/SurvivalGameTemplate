// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interactables/SInventoryData.h"
#include "SInventoryComponent.generated.h"



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API USInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USInventoryComponent();


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory")
	float MaxWeight = 200.f;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	float CurrentWeight;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	TArray<FName> Inventory_ItemIDs;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Inventory")
	TArray<FInventoryData> Inventory_ItemData;


public:

	bool AddItem(FInventoryData ItemData);

	bool RemoveItem(FInventoryData ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool BP_AddItem(FInventoryData ItemData);

	UFUNCTION(BlueprintCallable, Category = "Inventory")
	bool BP_RemoveItem(FInventoryData ItemData);

protected:

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_AddItem(FInventoryData ItemData);
	bool Server_AddItem_Validate(FInventoryData ItemData);
	void Server_AddItem_Implementation(FInventoryData ItemData);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_RemoveItem(FInventoryData ItemData);
	bool Server_RemoveItem_Validate(FInventoryData ItemData);
	void Server_RemoveItem_Implementation(FInventoryData ItemData);

	void AddItemData(FInventoryData ItemData);

	void RemoveItemData(FInventoryData ItemData);
	
};
