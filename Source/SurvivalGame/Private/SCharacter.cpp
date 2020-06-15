// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "SHealthComponent.h"
#include "SPlayerStateComponent.h"

#include "TimerManager.h"
#include "Net/UnrealNetwork.h"


#include "..\Public\SCharacter.h"

 

// Sets default values
ASCharacter::ASCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);


	//Add Components
	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	PlayerStateComponent = CreateDefaultSubobject<USPlayerStateComponent>(TEXT("PlayerStateComponent"));

	//Add Spring Arm for Camera
	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComp->SetupAttachment(RootComponent);
	SpringArmComp->bUsePawnControlRotation = true;


	//Add Camera Component
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComp->SetupAttachment(SpringArmComp);

	GetMovementComponent()->GetNavAgentPropertiesRef().bCanCrouch = true;
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
	GetCharacterMovement()->MaxWalkSpeedCrouched = DefaultCrouchSpeed;

	bIsSprinting = false;
	
}


// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	StartStateTimers();
}
	




// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


//  ------------- States -------------

void ASCharacter::StartStateTimers()
{
	
	if (PlayerStateComponent == nullptr) { UE_LOG(LogTemp, Warning, TEXT("Could not find USPlayerStateComponent on ASCharacter")); return; }

		float StaminaDecayInterval;
		StaminaDecayInterval = PlayerStateComponent->GetSprintStaminaDecrementInterval();

		GetWorld()->GetTimerManager().SetTimer(SprintingTimerHandle, this, &ASCharacter::HandleSprinting, StaminaDecayInterval, true);

}


// ------------- Movement -------------


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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, & ASCharacter::StartSprint);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ASCharacter::EndSprint);

}

void ASCharacter::AddControllerYawInput(float Val)
{
	Super::AddControllerYawInput(Val);

	YawInput = Val;

}

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

}

// Called from ACharacter
void ASCharacter::EndCrouch()
{
	UnCrouch();

}

void ASCharacter::StartSprint()
{
	
	if (PlayerStateComponent == nullptr) { UE_LOG(LogTemp, Warning, TEXT("Could not find USPlayerStateComponent on ASCharacter")); return; }

	if (PlayerStateComponent->GetStamina() >= 10.f) //TODO REFACTOR Sprinting Stam Stats into Character
	{
		bIsSprinting = true;
		PlayerStateComponent->ControlStaminaRegenTimer(false);
		GetCharacterMovement()->MaxWalkSpeed = DefaultRunSpeed;
	}
}

void ASCharacter::EndSprint()
{
	if (PlayerStateComponent == nullptr) { UE_LOG(LogTemp, Warning, TEXT("Could not find USPlayerStateComponent on ASCharacter")); return; }

	bIsSprinting = false;
	PlayerStateComponent->ControlStaminaRegenTimer(true);
	GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;

}

void ASCharacter::HandleSprinting()
{
	if (PlayerStateComponent == nullptr) { UE_LOG(LogTemp, Warning, TEXT("Could not find USPlayerStateComponent on ASCharacter")); return; }

	if (bIsSprinting)
	{
		PlayerStateComponent->LowerStamina(PlayerStateComponent->GetSprintStaminaDecrementValue());

		if (PlayerStateComponent->GetStamina() <= 10.f) //TODO change this to a variable
		{
			EndSprint();
		}
	}


}

