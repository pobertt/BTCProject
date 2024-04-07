// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Adding input mapping context // Checking if Player Controller is valid */
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		/* Getting local player subsystem // Checking if Enhanced Player Subsystem is valid */
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			//If valid add input context
			Subsystem->AddMappingContext(InputMapping, 0);
		}
	}

	/* Checked the input component and called the input test action // If triggered it calls function test input*/
	if (UEnhancedInputComponent* Input = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		Input->BindAction(TestAction, ETriggerEvent::Triggered, this, &APlayerCharacter::TestInput);
	}
}

void APlayerCharacter::TestInput()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Pressed input action");
}
