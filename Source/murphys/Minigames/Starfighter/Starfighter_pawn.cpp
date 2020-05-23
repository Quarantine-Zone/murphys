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
	
	apply_internal_forces(DeltaTime);

	// move fighter according to velocity
	FVector translation = Velocity * DeltaTime * 100;
	AddActorWorldOffset(translation);

}

// Called to bind functionality to input
void AStarfighter_pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStarfighter_pawn::input_SetThrottle);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStarfighter_pawn::input_SetRoll);
	PlayerInputComponent->BindAxis("Turn", this, &AStarfighter_pawn::input_SetYaw);
	PlayerInputComponent->BindAxis("LookUp", this, &AStarfighter_pawn::input_SetPitch);
	//set up pause menu
	//PlayerInputComponent->BindAction("OpenInGameMenu", IE_Pressed, this, &AStarfighter_pawn::LoadMiniGameMenu);
		
}

// rotates the ship based on control input
void AStarfighter_pawn::apply_Rotation(float DeltaTime)
{
	// NOTE: I may want to simulate this as well so that
	// ship mass also affects a ship's rotation speed
	// in adition to its acceleration.

	// Apply Rotation to Ship
	float rotX = MaxRotation * DeltaTime * yaw;
	float rotY = MaxRotation * DeltaTime * pitch;
	float rotZ = MaxRotation * DeltaTime * roll;

	FQuat RotationX(GetActorUpVector(), FMath::DegreesToRadians(rotX));
	AddActorWorldRotation(RotationX);

	FQuat RotationY(GetActorRightVector(), FMath::DegreesToRadians(rotY));
	AddActorWorldRotation(RotationY);

	FQuat RotationZ(GetActorForwardVector(), FMath::DegreesToRadians(-1*rotZ));
	AddActorWorldRotation(RotationZ);

	ForwardVector = GetActorForwardVector(); // idk if this is what I want
}

// updates the velocity based on forces applied by ship on itself
void AStarfighter_pawn::apply_internal_forces(float DeltaTime)
{
	// apply engine thrust
	if (ForwardThrustersEnabled) 
	{
		Velocity +=  DeltaTime * (ForwardVector * throttle * MaxForwardThrust) / Mass;
	}

	// apply inertial damping
	if (InertialDampingEnabled) {
		FVector DampingVector;
		if (ForwardThrustersEnabled && throttle != 0) {
			DampingVector = -1 * (Velocity - ForwardVector.ProjectOnTo(Velocity).GetSafeNormal());
		}
		else
		{
			DampingVector = -1 * Velocity.GetSafeNormal();
		}
		Velocity += DeltaTime * InertialDamperStrength * DampingVector;
	}

}

//player input functions
void AStarfighter_pawn::input_SetThrottle(float value)
{
	throttle = value;
}

void AStarfighter_pawn::input_SetRoll(float value)
{
	
	roll = value;
}

void AStarfighter_pawn::input_SetYaw(float value)
{
	yaw = value;
}

void AStarfighter_pawn::input_SetPitch(float value)
{
	pitch = value;
}

/*
void AStarfighter_pawn::getMouseRotation() 
{
	float MouseX, MouseY;
	GetController()->GetMousePosition(&MouseX, &MouseY);
	
	
}
*/
//pause menu
/*
void AStarfighter_pawn::LoadMiniGameMenu()
{
	GetMurphysGameInstance()->LoadMiniGameMenu();
}
*/