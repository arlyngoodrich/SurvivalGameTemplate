// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"

#include "SHealthComponent.h"
#include "SPlayerStateComponent.h"
#include "Components/StaminaComponent.h"
#include "Components/SCharacterMovementComponent.h"

#include "TimerManager.h"
#include "Net/UnrealNetwork.h"


#include "..\Public\SCharacter.h"



ASCharacter::ASCharacter(const FObjectInitializer& ObjectInitializer)

	: Super(ObjectInitializer.SetDefaultSubobjectClass<USCharacterMovementComponent>(ACharacter::CharacterMovementComponentName))


	// Sets default values
{

	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetReplicates(true);


	//Add Components
	HealthComponent = CreateDefaultSubobject<USHealthComponent>(TEXT("HealthComponent"));
	StaminaComponent = CreateDefaultSubobject<UStaminaComponent>(TEXT("StaminaComponent"));

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

}


// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitializePlayerStats();

}


// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty >& OutLifetimeProps) const
{

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASCharacter, bIsSprinting);
}


//  ------------- States -------------

void ASCharacter::InitializePlayerStats()
{
	if (StaminaComponent == nullptr) { UE_LOG(LogTemp, Warning, TEXT("NO STAMINA COMPONENT!")); return; }

	GetWorld()->GetTimerManager().SetTimer(PlayerStatsUpdateTimerHandle, this, &ASCharacter::UpdatePlayerStats, 1.f, true);
}

void ASCharacter::UpdatePlayerStats()
{
	CheckStamina();
}

void ASCharacter::CheckStamina()
{
	// If sprinting but not enough stam, end sprint
	if (bIsSprinting == true && StaminaComponent->GetCurrentStamina() <= MinStamToSprint)
	{

	}
	else
	{

	}
}

void ASCharacter::UpdateStatCheckFrequency(float CheckFrequency)
{
	GetWorld()->GetTimerManager().ClearTimer(PlayerStatsUpdateTimerHandle);
	GetWorld()->GetTimerManager().SetTimer(PlayerStatsUpdateTimerHandle, this, &ASCharacter::UpdatePlayerStats, CheckFrequency, true);
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

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASCharacter::Jump);

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

void ASCharacter::Jump()
{
	//check if there is enough stam to jump, will remove and then trigger jump if true
	if (StaminaComponent->RequestOneTimeStaminaDrain(StaminaRequiredToJump) == true)
	{
		Super::Jump();
	}
}


void ASCharacter::StartSprint()
{
	
	if (StaminaComponent == nullptr) { UE_LOG(LogTemp, Warning, TEXT("Character could not find stamina component")); return; }


		if (GetLocalRole() < ROLE_Authority)
		{
			UE_LOG(LogTemp, Warning, TEXT("Client Start Sprinting"))
			Server_StartSprinting();
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = DefaultRunSpeed;
		}

}

void ASCharacter::EndSprint()
{
	if (StaminaComponent == nullptr) { UE_LOG(LogTemp, Warning, TEXT("Character could not find stamina component")); return; }


		if (GetLocalRole() < ROLE_Authority)
		{
			UE_LOG(LogTemp, Warning, TEXT("Client Stop Sprinting"))
			Server_EndSprinting();
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = DefaultWalkSpeed;
		}

}

bool ASCharacter::Server_StartSprinting_Validate()
{
	return true;
}

void ASCharacter::Server_StartSprinting_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Start Sprinting"));
	StartSprint();
}

bool ASCharacter::Server_EndSprinting_Validate()
{
	return true;
}

void ASCharacter::Server_EndSprinting_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("Server Stop Sprinting"));
	EndSprint();
}




