// Fill out your copyright notice in the Description page of Project Settings.


#include "Starfighter_pawn.h"

// Sets default values
AStarfighter_pawn::AStarfighter_pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStarfighter_pawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStarfighter_pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	apply_Rotation(DeltaTime);
	
	FVector force = MaxForce * throttle * GetActorForwardVector();
	FVector acceleration = force / mass;
	velocity += acceleration;

	// move fighter according to velocity
	FVector translation = velocity * DeltaTime * 100;
	AddActorWorldOffset(translation);

}

// Called to bind functionality to input
void AStarfighter_pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStarfighter_pawn::input_moveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStarfighter_pawn::MoveRight);

}


void AStarfighter_pawn::apply_Rotation(float DeltaTime)
{
	float RotationAngle = MaxDegreesPerSecond * DeltaTime * SteeringThrow;
	FQuat RotationDelta(GetActorUpVector(), FMath::DegreesToRadians(RotationAngle));

	velocity = RotationDelta.RotateVector(velocity);

	AddActorWorldRotation(RotationDelta);
}

//player input functions
void AStarfighter_pawn::input_moveForward(float value) 
{
	throttle = value;
}

void AStarfighter_pawn::MoveRight(float value)
{
	SteeringThrow = value;
}
