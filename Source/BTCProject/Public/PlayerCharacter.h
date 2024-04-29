// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "../../../Plugins/Runtime/CableComponent/Source/CableComponent/Classes/CableComponent.h"
#include "GrappledActor.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class BTCPROJECT_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	APlayerCharacter();

	UPROPERTY(EditAnywhere, Category = "Player Params")
		int HealthPoints = 500;

	UPROPERTY(EditAnywhere, Category = "Player Params")
		float Strength = 10;

	UPROPERTY(EditAnywhere, Category = "Player Params")
		float Armour = 3;

	UPROPERTY(EditAnywhere, Category = "Player Attack Params")
		float AttackRange = 6.0f;

	UPROPERTY(EditAnywhere, Category = "Player Attack Params")
		float AttackInterval = 1.2f;

	UPROPERTY(EditAnywhere, Category = "Player Attack Params")
		bool bCanAttack = true;

	UPROPERTY(EditAnywhere, Category = "Player Attack Params")
		bool bCanGrapple = true;

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter", meta = (DisplayName = "Get HP"))
		int GetHealthPoints();

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		bool IsKilled();

	UFUNCTION(BlueprintCallable, Category = "PlayerCharacter")
		bool CanAttack();

	void Hit(int damage);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int _HealthPoints;
	float _AttackCountingDown;

	void DieProcess();

	/* References to Input Mapping / Action */

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* AttackAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* GrappleAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Grappling", meta = (AllowPrivateAccess = "true"))
		class UCableComponent* GrappleCable;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* SprintAction;

	UPROPERTY(EditAnywhere, Category = "EnhancedInput")
		class UInputAction* CrouchAction;


	/* FInputActionValue to find out which button we are pressing*/

	void Move(const FInputActionValue& InputValue);

	void Jump();

	void StopJump();

	void Look(const FInputActionValue& InputValue);	

	void Attack();

	void Grapple();

	void StopGrapple();

	void Sprint();

	void StopSprint();

	void Crouch();

	void StopCrouch();

private:

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* _CameraComponent;

	UPROPERTY(VisibleAnywhere, Category = "Camera", meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* _SpringArmComponent;

	float MaxLineDistance = 1600.0f;

	bool isGrappling = false;

	FVector GrapplePoint;

public:

	FORCEINLINE USpringArmComponent* GetSpringArmComponent() const
	{
		return _SpringArmComponent;
	}

	FORCEINLINE UCameraComponent* GetCameraComponent() const
	{
		return _CameraComponent;
	}
};
