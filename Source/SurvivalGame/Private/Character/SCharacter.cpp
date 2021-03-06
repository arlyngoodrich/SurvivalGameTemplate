// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalGame/Public/Character/SCharacter.h"
#include "SurvivalGame/SurvivalGame.h"

//UE4 Includes
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "TimerManager.h"
#include "Net/UnrealNetwork.h"

//Custom Includes
#include "Components/SHealthComponent.h"
#include "Components/SStaminaComponent.h"
#include "Components/SCharacterMovementComponent.h"
#include "Components/SPlayerInteractionComponent.h"
#include "Interactables/BaseClasses/SBaseInteractable.h"
#include "Components/SInventoryComponent.h"
#include "Components/SHungerComponent.h"




ASCharacter::ASCharacter(const FObjectInitializer& ObjectInitializer)

	: Super(ObjectInitializer.SetDefaultSubobjectClass<USCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))


	// Sets default values
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);


	//Add Components
	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	StaminaComponent = CreateDefaultSubobject<USStaminaComponent>(TEXT("StaminaComponent"));
	InteractionComponent = CreateDefaultSubobject<USPlayerInteractionComponent>(TEXT("InteractionComponent"));
	InventoryComponent = CreateDefaultSubobject<USInventoryComponent>(TEXT("InventoryComponent"));
	HungerComponent = CreateDefaultSubobject<USHungerComponent>(TEXT("HungerComponent"));

	//Add Spring Arm for Camera
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;
	SpringArmComp->bEnableCameraLag = true;
	SpringArmComp->bEnableCameraRotationLag = true;

	//Add Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = DefaultCrouchSpeed;

}


// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	//Bind to death
	HealthComponent->OnDeath.AddDynamic(this, &ASCharacter::OnDeath);

}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, bWantsToSprint);
	DOREPLIFETIME(ASCharacter, bIsSprinting);
	DOREPLIFETIME(ASCharacter, bIsAlive);
}


//  ------------- Public Getters -------------

bool ASCharacter::GetWantsToSprint()
{
	if (!StaminaComponent) { return false; }
	if (StaminaComponent->GetCurrentStamina() > MinStamToSprint && bWantsToSprint)
	{return true;} else {return false;}
}

void ASCharacter::SetIsSprinting(bool IsSprinting) {bIsSprinting = IsSprinting;}

float ASCharacter::GetSprintSpeedModifier() {return SprintSpeedMuliplyer;}

float ASCharacter::GetDefaultWalkSpeed() {return DefaultWalkSpeed;}

USInventoryComponent* ASCharacter::GetInventoryComponent() {return InventoryComponent;}

USStaminaComponent* ASCharacter::GetStaminaComponent(){return StaminaComponent;}

USHungerComponent* ASCharacter::GetHungerComponent() {return HungerComponent;}

USHealthComponent* ASCharacter::GetHealthComponent() {return HealthComponent;}

float ASCharacter::GetMinStaminaToSprint() {return MinStamToSprint;}

// ------------- Movement -------------


//  ---- Public Movement Functions

void ASCharacter::InteruptSprint()
{
	if (bIsSprinting)
	{
		EndSprint();
	}
}



// Called to bind functionality to input
void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	
	PlayerInputComponent->BindAxis("MoveForward", this, &ASCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASCharacter::MoveRight);

	//Controller Functions Inherited from ACharacter
	PlayerInputComponent->BindAxis("LookUp", this, &ASCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn", this, &ASCharacter::AddControllerYawInput);

	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASCharacter::BeginCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASCharacter::EndCrouch);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, & ASCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::EndSprint);

	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &ASCharacter::TriggerInteract);
}

void ASCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val);

	YawInput = Val;
}


// ---- Protected Movement Functions


//Forward and Backward
void ASCharacter::MoveForward(float Value)
{
	AddMovementInput(GetActorForwardVector() * Value);
}

//Right and Left
void ASCharacter::MoveRight(float Value)
{
	AddMovementInput(GetActorRightVector() * Value);
}

// Called from ACharacter
void ASCharacter::BeginCrouch()
{
	Crouch();
	UE_LOG(LogDevelopment, Log, TEXT("Crouch"));

}

// Called from ACharacter
void ASCharacter::EndCrouch()
{
	UnCrouch();
	UE_LOG(LogDevelopment, Log, TEXT("UnCrouch"));

}

void ASCharacter::Jump()
{
	//check if there is enough stam to jump, will remove and then trigger jump if true
	if (StaminaComponent->RequestOneTimeStaminaDrain(StaminaRequiredToJump) == true)
	{
		Super::Jump();
		UE_LOG(LogDevelopment, Log, TEXT("Jump"));
	}

	UE_LOG(LogDevelopment, Log, TEXT("Jump denied"));
}

void ASCharacter::OnDeath()
{
	//Tell Client to die... 
	if (GetLocalRole() < ROLE_Authority) {Client_OnDeath();}

	UE_LOG(LogDevelopment, Log, TEXT("Died"))


	InteruptSprint();
	GetMovementComponent()->StopMovementImmediately();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	bIsAlive = false;

	DetachFromControllerPendingDestroy();
	SetLifeSpan(2.f);

}
  
void ASCharacter::Client_OnDeath_Implementation()
{
	OnDeath();
}


void ASCharacter::StartSprint()
{
	
	if (StaminaComponent == nullptr) { UE_LOG(LogDevelopment, Error, TEXT("Character could not find stamina component")); return; }

	if (GetLocalRole() < ROLE_Authority)
	{
		Server_StartSprinting();
	}
	else
	{
		StaminaComponent->RequestStartStaminaDecay(SprintStamDecayRate);
		bWantsToSprint = true;
		UE_LOG(LogDevelopment, Log, TEXT("Start Sprinting"));
	}
}

void ASCharacter::EndSprint()
{
	if (StaminaComponent == nullptr) { UE_LOG(LogDevelopment, Error, TEXT("Character could not find stamina component")); return; }

	if (GetLocalRole() < ROLE_Authority)
	{
		Server_EndSprinting();
	}
	else
	{
		StaminaComponent->RequestStopStaminaDecay();
		bWantsToSprint = false;
		UE_LOG(LogDevelopment, Log, TEXT("End Sprinting"));
	}

}

bool ASCharacter::Server_StartSprinting_Validate()
{
	return true;
}

void ASCharacter::Server_StartSprinting_Implementation()
{
	StartSprint();
}

bool ASCharacter::Server_EndSprinting_Validate()
{
	return true;
}

void ASCharacter::Server_EndSprinting_Implementation()
{
	EndSprint();
}

// ----- Player Interaction -----

void ASCharacter::TriggerInteract()
{
	if (InteractionComponent->GetIsInteractableInView() == false) { return; }
	Interact(InteractionComponent->GetInteractableInView());
}

void ASCharacter::Interact(ASBaseInteractable* Interactable)
{
	
	if (Interactable == nullptr) { return; }

	//If no interactable in view, don't do anything
	if (GetLocalRole() < ROLE_Authority)
	{
		Server_Interact(Interactable);
	}
	else
	{
		UE_LOG(LogDevelopment, Log, TEXT("Player called interact"))

			if (Interactable == nullptr) { return; }
			Interactable->OnInteract(this);
	}

}

bool ASCharacter::Server_Interact_Validate(ASBaseInteractable* Interactable)
{
	return true;
}

void ASCharacter::Server_Interact_Implementation(ASBaseInteractable* Interactable)
{
	Interact(Interactable);
}




