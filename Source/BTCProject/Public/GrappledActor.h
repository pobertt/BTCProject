// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GrappledActor.generated.h"

UCLASS()
class BTCPROJECT_API AGrappledActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGrappledActor();


	FORCEINLINE UBoxComponent* GetBoxComponent() const
	{
		return _BoxComponent;
	}

	FORCEINLINE UStaticMeshComponent* GetMeshComponent() const
	{
		return _MeshComponent;
	}

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Component", meta = (AllowPrivateAccess = "true"))
		UBoxComponent* _BoxComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Actor Component", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* _MeshComponent;

};
