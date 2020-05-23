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

	//=====================================================================
	// Ship Control Inputs
	//=====================================================================
	float throttle; // speed control input
	float roll; // roll controll input
	// pitch and yaw will be controlled by mouse input (somewhat tbd)
	float pitch; // mouse y
	float yaw; // mouse x

	bool InertialDampingEnabled = true;
	bool ForwardThrustersEnabled = true;
	//====================================================================
	// Ship Motion/Physics
	//====================================================================
	FVector Velocity; // the ship's current direction of motion
	FVector ForwardVector; // the direction the ship is currenty facing
	//====================================================================
	// Ship Stats
	//====================================================================
	UPROPERTY(EditAnywhere)
	float Mass = 100;
	
	UPROPERTY(EditAnywhere)			
	float MaxForwardThrust = 10000;	// The maximum force per second the 
	// engine is capable of applying to the ship.

	UPROPERTY(EditAnywhere)			 
	float MaxRotation = 90;	// The maximum degrees per second the ship is 
	// capable of rotating.

	UPROPERTY(EditAnywhere)				  
	float InertialDamperStrength = 10;	 // the strength of the inertial
	// Damper, a fictional subsystem that applies a force oposite of the 
	// ship's current velocity when the throttle is set to 0, and otherwise
	// applies a force oposite to any components of the ships velocity
	//  purpendicular to the ship's current forward vector.
	//
	// this subsystem is intended to make piloting in combat easier, 
	// and can be easily turned on and off by the ship's pilot


	//====================================================================


	// pysics simulation functions
	void apply_Rotation(float DeltaTime);
	void apply_internal_forces(float DeltaTime);


	//Input Axis Mappings
	void input_SetThrottle(float value);
	void input_SetPitch(float value);
	void input_SetRoll(float value);
	void input_SetYaw(float value);

	void getMouseRotation();

	//Input Action Mappings
	//UFUNCTION(BlueprintCallable)
	//void LoadMiniGameMenu();

};
