// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Starfighter_pawn.generated.h"

UCLASS()
class MURPHYS_API AStarfighter_pawn : public APawn
{
	GENERATED_BODY()

	// Sets default values for this pawn's properties
	AStarfighter_pawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	UPROPERTY(EditAnywhere)
	float mass = 1000; // kg

	UPROPERTY(EditAnywhere)
	float MaxForce = 1000;

	UPROPERTY(EditAnywhere)
	float MaxDegreesPerSecond = 90;

	float throttle;
	FVector velocity; // m/s
	float SteeringThrow;

	void input_moveForward(float value);
	void MoveRight(float Value);
	void apply_Rotation(float DeltaTime);
};
