// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include "InputMappingContext.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Engine/World.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "DrawDebugHelpers.h"
#include "PlayerCharacterAnimInstance.h"


// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	

	_SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	_SpringArmComponent->SetupAttachment(RootComponent);
	_SpringArmComponent->bUsePawnControlRotation = true;

	_CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	_CameraComponent->SetupAttachment(_SpringArmComponent);
	//Was RootComponent for (FPS) and below was for fps cam
	//Camera->bUsePawnControlRotation = true;

	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;

	GrappleCable = CreateDefaultSubobject<UCableComponent>(TEXT("Grappling Line"));
	GrappleCable->SetupAttachment(_CameraComponent);
	GrappleCable->SetVisibility(false);

	
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

	UE_LOG(LogTemp, Error, TEXT("Capsule Component %s"), *this->RootComponent.GetFullName());

}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UPlayerCharacterAnimInstance* animInst = animInst = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	animInst->Speed = GetCharacterMovement()->Velocity.Size2D();

	//Remove out of event tick

	WallSlide();
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

		Input->BindAction(JumpAction, ETriggerEvent::Started, this, &APlayerCharacter::Jump);
		Input->BindAction(JumpAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopJump);

		Input->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);

		Input->BindAction(AttackAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Attack);

		Input->BindAction(GrappleAction, ETriggerEvent::Started, this, &APlayerCharacter::Grapple);
		Input->BindAction(GrappleAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopGrapple);

		Input->BindAction(SprintAction, ETriggerEvent::Started, this, &APlayerCharacter::Sprint);
		Input->BindAction(SprintAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopSprint);

		Input->BindAction(CrouchAction, ETriggerEvent::Started, this, &APlayerCharacter::Crouch);
		Input->BindAction(CrouchAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopCrouch);
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

//Character Velocity + 

void APlayerCharacter::Jump()
{
	if (bInWallSlide)
	{

		double Xresult = GetActorForwardVector().X * WallJumpForce;

		double Yresult = GetActorForwardVector().Y * WallJumpForce;

		float WallJumpVerticalForce = 1000.0f;

		ACharacter::LaunchCharacter(FVector(Xresult, Yresult, GetMovementComponent()->Velocity.Z + WallJumpVerticalForce), true, true);

	}
	else
	{
		if (JumpCount < ACharacter::JumpMaxCount)
		{
			if (GetCharacterMovement()->IsFalling())
			{
				//Animation Montage for double jump
				//https://www.youtube.com/watch?v=_flv0-uYD60&list=PL9z3tc0RL6Z5Yi7-W8qxjrzTb6tHS_UAK&index=6

				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "Double Jump");

				FVector ResetVelocity(ACharacter::GetVelocity().X, ACharacter::GetVelocity().Y, 0.0);

				GetCharacterMovement()->Velocity = ResetVelocity;

				ACharacter::LaunchCharacter(FVector(GetMovementComponent()->Velocity.X, GetMovementComponent()->Velocity.Y, GetMovementComponent()->Velocity.Z + AirJumpForce), true, true);

				JumpCount++;
			}
			else
			{
				ACharacter::Jump();

				JumpCount++;

				//Add jumping animation here
			}
		}
	}
}

void APlayerCharacter::StopJump()
{
	ACharacter::StopJumping();
}

void APlayerCharacter::Look(const FInputActionValue& InputValue)
{
	FVector2D InputVector = InputValue.Get<FVector2D>();

	if (IsValid(Controller))
	{
		AddControllerYawInput(InputVector.X);
		AddControllerPitchInput(InputVector.Y);

		LaunchDirection = FVector2D(InputVector.X, InputVector.Y);
	}
}

void APlayerCharacter::Attack()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Pressed input action");

	//UPlayerCharacterAnimInstance* animInst = animInst = Cast<UPlayerCharacterAnimInstance>(GetMesh()->GetAnimInstance());
	//animInst->State = EPlayerState::Attack;

}

void APlayerCharacter::Grapple()
{
	/*
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FVector Start = (GetActorForwardVector() * 50.f) + GetCapsuleComponent()->GetComponentLocation();
	FVector End = Start + (MaxLineDistance * UKismetMathLibrary::GetForwardVector(_CameraComponent->GetComponentRotation()));

	DrawDebugLine(GetWorld(), Start, End, FColor::Emerald);

	FHitResult HitResult;

	bool HasHit = GetWorld()->SweepSingleByChannel(HitResult, Start, End,
		FQuat::Identity, ECC_GameTraceChannel2, FCollisionShape::MakeSphere(1.0f), Params);
	
	if (HasHit && bCanGrapple == true)
	{
		isGrappling = true;
		bCanGrapple = false;

		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
		GrappleCable->SetVisibility(true);
		GrapplePoint = HitResult.ImpactPoint;

		if (isGrappling)
		{
			GrappleCable->EndLocation = GetActorTransform().InverseTransformPosition(GrapplePoint);
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, FString::FormatAsNumber(GrappleCable->EndLocation.X));

			GetCharacterMovement()->AddImpulse((GrapplePoint - GetActorLocation()).GetSafeNormal() * 10000);
		}
	}
	*/

	/*
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	FVector Start = (GetCameraComponent()->GetComponentLocation());

	//float is distance we want to grab

	FVector End = (GetCameraComponent()->GetForwardVector() * 3000.0f);

	FHitResult HitResult;

	bool HasHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, Start + End, ECC_WorldDynamic, Params);

	if (HasHit)
	{
		
	}
	*/
}

void APlayerCharacter::StopGrapple()
{
	isGrappling = false;
	if (!GetCharacterMovement()->IsFalling())
	{
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Falling);
	}
	GrappleCable->SetVisibility(false);
	bCanGrapple = true;
}

void APlayerCharacter::Sprint()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Pressed Sprint action");

	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void APlayerCharacter::StopSprint()
{
	GetCharacterMovement()->MaxWalkSpeed = 300.0f;
}

void APlayerCharacter::Crouch()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Pressed Crouch action");

	ACharacter::Crouch();
}

void APlayerCharacter::StopCrouch()
{
	GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, "Stop Crouch action");

	ACharacter::UnCrouch();
}

void APlayerCharacter::WallSlide()
{

	float Time = GetWorld()->GetDeltaSeconds();

	TArray< TEnumAsByte< EObjectTypeQuery > > Actors;
	Actors.Add(EObjectTypeQuery::ObjectTypeQuery1);

	TArray<AActor*, FDefaultAllocator> IgnoreActors;

	FCollisionObjectQueryParams ObjectParams;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FVector Start = GetActorLocation();
	FVector End = GetActorLocation();

	TArray<FHitResult> HitResult;

	if (GetCharacterMovement()->IsFalling())
	{
		bool HasHit = UKismetSystemLibrary::CapsuleTraceMultiForObjects
		(GetWorld(), AActor::GetActorLocation(), AActor::GetActorLocation(), 35.0f, 89.0f,
			Actors, false, IgnoreActors, EDrawDebugTrace::ForOneFrame,
			HitResult, true, FLinearColor::Red, FLinearColor::Green, 5.0f);

		if (HasHit)
		{
			bInWallSlide = true;

			if (GetCharacterMovement()->Velocity.Z <= 0)
			{
				GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Blue, "Has Hit");

				if (bInWallSlide == true)
				{
					

					JumpCount = ACharacter::JumpMaxCount - 1;

					GetCharacterMovement()->Velocity.X = APlayerCharacter::GetVelocity().X;

					GetCharacterMovement()->Velocity.Y = APlayerCharacter::GetVelocity().Y;
				}

				GetCharacterMovement()->GravityScale = 0.1;

				GetCharacterMovement()->Velocity = UKismetMathLibrary::VInterpTo(
					APlayerCharacter::GetVelocity(),
					FVector(0.0,0.0,GetActorForwardVector().Z - 50),
					Time,
					8);

				//Last value is Wall Slide Deceleration
			}

			//Do Once
			SetActorRotation(GetActorRotation().Add(0, 180, 0), ETeleportType::None);

			//Wall Slide animation
		}
		else
		{
			GetCharacterMovement()->GravityScale = DefaultGravity;

			bInWallSlide = false;

			GetCharacterMovement()->Velocity.X = APlayerCharacter::GetVelocity().X;

			GetCharacterMovement()->Velocity.Y = APlayerCharacter::GetVelocity().Y;
		}
	}
}


/*
	Bug:Because of SetActorRotation being constantly called from event tick- 
		when trying to wall jump it will send the player upwards IF by chance the player is facing into the wall
		Need to make the the SetActorRotation only happen once
		Maybe only do that in bp?
*/