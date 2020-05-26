// Fill out your copyright notice in the Description page of Project Settings.


#include "Starfighter_pawn.h"
#include "StarfighterPlayerController.h" 

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


	//UpdateSteeringVector();
	
	//UE_LOG(LogTemp, Warning, TEXT("Pitch= %f, Yaw=%f"), pitch, yaw);

	//compress axis value using a sigmoid function
	float compressed_yaw   = 2 * ((1 / (1 + exp(-1 * yaw))) - 0.5);
	float compressed_pitch = 2 * ((1 / (1 + exp(-1 * pitch))) - 0.5);

	UE_LOG(LogTemp, Warning, TEXT("Compressed Pitch= %f, Compressed Yaw=%f"), compressed_pitch, compressed_yaw);
	
	float s = 70.0;
	SteeringVectorUI = FVector2D(compressed_yaw*s, compressed_pitch*s);

	// Apply Rotation to Ship
	
	float rotX = MaxRotation * DeltaTime * compressed_yaw;
	float rotY = MaxRotation * DeltaTime * compressed_pitch;
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
	GetController();
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
	yaw += value;
	//if (yaw < .2 && yaw > -.2) yaw = 0;

}

void AStarfighter_pawn::input_SetPitch(float value)
{
	
	pitch += value;
	//if (pitch < .2 && pitch > -.2) pitch = 0;

}

void AStarfighter_pawn::UpdateSteeringVector()
{
	/*
	UWorld *world = GetWorld(); 
	if (!ensure(world != nullptr)) return;
	AStarfighterPlayerController* SF_player = (AStarfighterPlayerController*) world->GetFirstPlayerController();

	FVector2D SteeringVector = SF_player->GetSteeringVector();

	float x = SteeringVector.X;
	float y = SteeringVector.Y;

	//SteeringVector.GetSafeNormal();

	float sigmoid_X = 1 / (1 + exp(-1 * x));
	float sigmoid_Y = 1 / (1 + exp(-1 * y));



	yaw = sigmoid_X;
	pitch = sigmoid_Y;
	*/

	//=========================================


}

