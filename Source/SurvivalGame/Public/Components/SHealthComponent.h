// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SHealthComponent.generated.h"


//////////// ===== Please Note ====== \\\\\\\\\\\\
// This class should not be dependent on any others besides actor for maximum flexibility.  
// It should be able to be used by everthing from a wall to a character 




class ASCharacter;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnHealthChange, float, NewHealth);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnDeath);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SURVIVALGAME_API USHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USHealthComponent();

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnHealthChange OnHealthChange;

	UPROPERTY(BlueprintAssignable, Category = "Health")
	FOnDeath OnDeath;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void Initalize();

	UFUNCTION()
	void OnOwnerTakeDamange(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);

	UPROPERTY(ReplicatedUsing = OnRep_HealthChanged, BlueprintReadOnly, Category = "Health")
	float Health;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float DefaultHealth;

	UFUNCTION()
	void OnRep_HealthChanged();

};
