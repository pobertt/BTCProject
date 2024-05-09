// Fill out your copyright notice in the Description page of Project Settings.


#include "GrappledActor.h"

// Sets default values
AGrappledActor::AGrappledActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Static Mesh"));
	SetRootComponent(_MeshComponent);

	_BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("Box Collision"));
	_BoxComponent->SetupAttachment(_MeshComponent);

	this->Tags.Add(FName("Grappleable"));
}

// Called when the game starts or when spawned
void AGrappledActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGrappledActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

