// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SInventoryData.generated.h"

USTRUCT(BlueprintType)
struct FInventoryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	FText DisplayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	FText Discription;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	float ItemWeight;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	UTexture2D* ItemIcom;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	UClass* InWorldClassReference;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Item Data")
	bool bCanBeUsedFromInventory;

};