// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInventoryData.generated.h"

class USGameplayEffect;
class USkeletalMeshComponent;

UENUM(BlueprintType)
enum class EItemType : uint8
{
	//When used will effect the character's base stats
	IT_Consumable	UMETA(DisplayName="Consumable"),
	//When used will be added to the player's SKM
	IT_Equipable	UMETA(DisplayName="Equipable"),
	//Should be considered as abstract and used as a currency to do something in the game world
	IT_Resource		UMETA(DisplayName="Resource")

};

UENUM(BlueprintType)
enum class EItemSize : uint8
{
	IS_Small	UMETA(DisplayName = "Small"),
	IS_Medium	UMETA(DisplayName = "Medium"),
	IS_Large	UMETA(DisplayName = "Large")
};


UENUM(BlueprintType)
enum class EItemHealthDecayType : uint8
{
	IH_None		UMETA(DisplayName = "None"),
	IH_Slow		UMETA(DisplayName = "Slow"),
	IH_Medium	UMETA(DisplayName = "Medium"),
	IH_Fast		UMETA(DisplayName = "Fast"),

};


USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	FName ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	FText Discription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	UTexture2D* ItemIcom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	bool bCanBeUsedFromInventory;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	bool bCanBeDropped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	bool bCanBeStacked;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data", meta = (ClampMin = 0, ClmapMax = 100))
	float ItemHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data", meta = (ClampMin = 0))
	float ItemWeight;

	// will be ignored if DecayAmountPerInterval is set to 0
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	EItemHealthDecayType DecayType;

	// Will be ignored if Health Decay type set to None
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	float DecayAmountPerInterval;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	EItemSize ItemSize;

	//Triggered when an item is used from an inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	TSubclassOf<USGameplayEffect> GamePlayEffect;

	//Only needed if is an equipment type item
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	USkeletalMeshComponent* SkeletalMesh;

};