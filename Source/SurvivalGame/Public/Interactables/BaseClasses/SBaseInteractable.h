// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SBaseInteractable.generated.h"

class UStaticMeshComponent;

UCLASS()
class SURVIVALGAME_API ASBaseInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASBaseInteractable();

	virtual void OnInteract(AActor* InteractingActor);

	virtual void OnFocus();

	virtual void OnLostFocus();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* BaseStaticMesh;

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "OnFocus"))
	void BP_OnFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "OnLostFocus"))
	void BP_OnLostFocus();

	UFUNCTION(BlueprintImplementableEvent, Category = "Interaction", meta = (DisplayName = "OnInteracted"))
	void BP_OnInteracted(AActor* InteractingActor);

};
