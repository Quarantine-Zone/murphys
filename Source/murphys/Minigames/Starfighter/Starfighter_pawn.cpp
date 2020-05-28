// Fill out your copyright notice in the Description page of Project Settings.


#include "Starfighter_pawn.h"
#include "Projectile.h"
#include "StarfighterPlayerController.h" 
#include "DrawDebugHelpers.h"

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

	LastFiredTime = GetWorld()->GetTimeSeconds();
}

void AStarfighter_pawn::SetFighterMeshReference(UStaticMeshComponent* FighterMeshReference)
{
	FighterMesh = FighterMeshReference;
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

	//Handle weapons systems
	
	//

	if (MainWeaponEnabled)
	{
		auto time = GetWorld()->GetTimeSeconds();
		if (time - LastFiredTime > MainWeaponFireRate) 
		{
			FireMainWeapon();
			LastFiredTime = time;
		}
	}

	if (ShowDebugInfo) 
	{
		DrawDebugString(GetWorld(), FVector(0, 0, 100), GetEnumText(Role), this, FColor::White, DeltaTime);
	}

}

// Called to bind functionality to input
void AStarfighter_pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AStarfighter_pawn::input_SetThrottle);
	PlayerInputComponent->BindAxis("MoveRight", this, &AStarfighter_pawn::input_SetRoll);
	PlayerInputComponent->BindAxis("Turn", this, &AStarfighter_pawn::input_SetYaw);
	PlayerInputComponent->BindAxis("LookUp", this, &AStarfighter_pawn::input_SetPitch);
	// implement controller  joystick support

	// Action Mappings TBD

	//InertialDamper On/Off
	//Engine On/Off
	//Beam Hook On/Off
	//Fire Lasers
	PlayerInputComponent->BindAction("ActionLMB", IE_Pressed, this, &AStarfighter_pawn::input_Activate_MainWeapon);
	PlayerInputComponent->BindAction("ActionLMB", IE_Released, this, &AStarfighter_pawn::input_Deactivate_MainWeapon);

}

// rotates the ship based on control input
void AStarfighter_pawn::apply_Rotation(float DeltaTime)
{
	// NOTE: I may want to simulate this as well so that
	// ship mass also affects a ship's rotation speed
	// in adition to its acceleration.

	//compress axis value using a sigmoid function
	float compressed_yaw   = 2 * ((1 / (1 + exp(-1 * yaw))) - 0.5);
	float compressed_pitch = 2 * ((1 / (1 + exp(-1 * pitch))) - 0.5);

	//UE_LOG(LogTemp, Warning, TEXT("Compressed Pitch= %f, Compressed Yaw=%f"), compressed_pitch, compressed_yaw);
	
	float scale_factor = 70.0;
	SteeringVectorUI = FVector2D(compressed_yaw*scale_factor, compressed_pitch*scale_factor);

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

void AStarfighter_pawn::FireMainWeapon()
{
	

	FName SocketName;
	if (LeftFiredLast)
	{
		SocketName = FName("RightLaserSocket");
		LeftFiredLast = false;
	}
	else
	{
		SocketName = FName("LeftLaserSocket");
		LeftFiredLast = true;
	}

	
	AProjectile* Proj = GetWorld()->SpawnActor<AProjectile>(MainWeaponProjectileBlueprint,
								   FighterMesh->GetSocketLocation(SocketName),
								   FighterMesh->GetSocketRotation(SocketName));
	Proj->LaunchProjectile();
}
//========================================================================
//player input functions
//========================================================================
// Client Input Functions
void AStarfighter_pawn::input_SetThrottle(float value)
{
	throttle = value;
	Server_SetThrottle(value);
}

void AStarfighter_pawn::input_SetPitch(float value)
{
	pitch += value;
	Server_SetPitch(value);
}

void AStarfighter_pawn::input_SetRoll(float value)
{
	roll = value;
	Server_SetRoll(value);
}

void AStarfighter_pawn::input_SetYaw(float value)
{
	yaw += value;
	Server_SetYaw(value);
}

// Server Replication Functions
bool AStarfighter_pawn::Server_SetThrottle_Validate(float value)
{
	return (value >= -1 && value <= 1);
}

void AStarfighter_pawn::Server_SetThrottle_Implementation(float value)
{
	throttle = value;
}

bool AStarfighter_pawn::Server_SetRoll_Validate(float value)
{
	return (value >= -1 && value <= 1);
}

void AStarfighter_pawn::Server_SetRoll_Implementation(float value)
{
	roll = value;
}

bool AStarfighter_pawn::Server_SetYaw_Validate(float value)
{
	return (value >= -1 && value <= 1);
}

void AStarfighter_pawn::Server_SetYaw_Implementation(float value)
{
	yaw += value;
}

bool AStarfighter_pawn::Server_SetPitch_Validate(float value)
{
	return (value >= -1 && value <= 1);
}

void AStarfighter_pawn::Server_SetPitch_Implementation(float value)
{
	pitch += value;
}
//=============================================================================
//Input Action Functions
//=============================================================================
void AStarfighter_pawn::input_Activate_MainWeapon()
{
	MainWeaponEnabled = true;
	Server_Toggle_MainWeapon(true);
}

void AStarfighter_pawn::input_Deactivate_MainWeapon()
{
	MainWeaponEnabled = false;
	Server_Toggle_MainWeapon(false);
}

// server replication
bool AStarfighter_pawn::Server_Toggle_MainWeapon_Validate(bool enabled)
{
	return true;
}

void AStarfighter_pawn::Server_Toggle_MainWeapon_Implementation(bool enabled)
{
	MainWeaponEnabled = enabled;
}
//==============================================================================
//debug helpers
//==============================================================================
FString AStarfighter_pawn::GetEnumText(ENetRole netRole)
{
	switch (netRole)
	{
	case ROLE_None:
		return "None";
	case ROLE_SimulatedProxy:
		return "SimulatedProxy";
	case ROLE_AutonomousProxy:
		return "AutonomousProxy";
	case ROLE_Authority:
		return "Authority";
	default:
		return "ERROR";
	}
}