// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class BTCPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* SpringArm;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/* References to Input Mapping / Action */

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* LookAction;

	/* FInputActionValue to find out which button we are pressing*/

	void Move(const FInputActionValue& InputValue);

	void Jump();

	void Look(const FInputActionValue& InputValue);	
};
