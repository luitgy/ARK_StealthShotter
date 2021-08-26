// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/Player/STS_PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

//===============================================================================================================
ASTS_PlayerCharacter::ASTS_PlayerCharacter()
{

	SpringComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringComponent"));
	SpringComponent->SetupAttachment(RootComponent);
	SpringComponent->bUsePawnControlRotation = true;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringComponent);

}

//===============================================================================================================
void ASTS_PlayerCharacter::MoveForward(float AxisValue)
{
	AddMovementInput(GetActorForwardVector() * AxisValue);
}

//===============================================================================================================
void ASTS_PlayerCharacter::MoveRight(float AxisValue)
{
	AddMovementInput(GetActorRightVector() * AxisValue);
}

//===============================================================================================================
void ASTS_PlayerCharacter::StartCrouch()
{
	Crouch();
}

//===============================================================================================================
void ASTS_PlayerCharacter::EndCrouch()
{
	UnCrouch();
}

//===============================================================================================================
// Called to bind functionality to input
void ASTS_PlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	//AXIS
	PlayerInputComponent->BindAxis("LookUp", this, &ASTS_PlayerCharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookSide", this, &ASTS_PlayerCharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("MoveForward", this, &ASTS_PlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ASTS_PlayerCharacter::MoveRight);

	//ACTION
	PlayerInputComponent->BindAction("Crouch", IE_Pressed, this, &ASTS_PlayerCharacter::StartCrouch);
	PlayerInputComponent->BindAction("Crouch", IE_Released, this, &ASTS_PlayerCharacter::EndCrouch);

}
