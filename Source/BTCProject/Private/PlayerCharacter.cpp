// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerCharacterAnimInstance.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	_SpringArmComponent->SetupAttachment(RootComponent);
	_SpringArmComponent->bUsePawnControlRotation = true;

	_CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	_CameraComponent->SetupAttachment(_SpringArmComponent);
	//Was RootComponent for (FPS) and below was for fps cam
	//Camera->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

int APlayerCharacter::GetHealthPoints()
{
	return _HealthPoints;
}

bool APlayerCharacter::IsKilled()
{
	return (_HealthPoints <= 0.0f);
}

bool APlayerCharacter::CanAttack()
{
	UPlayerCharacterAnimInstance* animInst = animInst = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());

	return (_AttackCountingDown <= 0.0f && animInst->State == EPlayerState::Locomotion);
}

void APlayerCharacter::Hit(int damage)
{

}

void APlayerCharacter::DieProcess()
{

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

	UPlayerCharacterAnimInstance* animInst = animInst = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->Speed = GetCharacterMovement()->Velocity.Size2D();

	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FVector Start = GetActorLocation() + FVector(0,0,50);
	FVector End = Start + _CameraComponent->GetForwardVector() * 1000.f;

	bool HitSomething = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params, FCollisionResponseParams());


	if (HitSomething)
	{
		if (HitResult.GetActor()->ActorHasTag("Grappleable"))
		{
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Green, "Slayy queen");

			bCanGrapple = true;
		}
		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor(100, 0, 0));
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), End, 5, 5, FLinearColor::White);
	}
	else
	{
		bCanGrapple = false;

		UKismetSystemLibrary::DrawDebugLine(GetWorld(), Start, End, FColor(100, 0, 0));
		UKismetSystemLibrary::DrawDebugSphere(GetWorld(), End, 5, 5, FLinearColor::White);
	}
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
		Input->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		Input->BindAction(JumpAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Jump);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);
	}
}

void APlayerCharacter::Move(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		//Get forward direction
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		//If pressing W want to move in this direction:
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		//If pressing D want to move in this direction:
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		//Add movement input
		/* 
			Gets forward facing direction and then right facing direction 
			Then moving depending on input
			(W or S = forward or backwards)
			(A or D = Left or Right)
		*/

		AddMovementInput(ForwardDirection, InputVector.Y);
		AddMovementInput(RightDirection, InputVector.X);
	}
}

void APlayerCharacter::Jump()
{
	ACharacter::Jump();

	//Add jumping animation here
}

void APlayerCharacter::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);
	}
}

void APlayerCharacter::Attack()
{
	if(bCanGrapple == true)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Pressed input action");
		
		GetCapsuleComponent()->AddImpulse(GetActorForwardVector() * 500);
	}

	//UPlayerCharacterAnimInstance* animInst = animInst = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	//animInst->State = EPlayerState::Attack;

}