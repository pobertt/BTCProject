// Fill out your copyright notice in the Description page of Project Settings.


#include "Game_Plate.h"

// Sets default values
AGame_Plate::AGame_Plate()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGame_Plate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGame_Plate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGame_Plate::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

