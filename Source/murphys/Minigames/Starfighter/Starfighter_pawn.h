// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Projectile.h"
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
	
	UFUNCTION(BlueprintCallable)
	void SetFighterMeshReference(UStaticMeshComponent* FighterMeshReference);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//Used by the HUD to Animate the targeting reticle 
	UPROPERTY(BlueprintReadWrite)
	FVector2D SteeringVectorUI;
	//=====================================================================
	// Ship Health
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CurrentHealth = MaxHealth;
	//=====================================================================
private:
	//=====================================================================
	bool ShowDebugInfo = false;
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
	bool MainWeaponEnabled = false;
	//====================================================================
	// Ship Motion/Physics
	//====================================================================
	FVector Velocity; // the ship's current direction of motion
	FVector ForwardVector; // the direction the ship is currenty facing
	//====================================================================
	// Ship Stats
	//====================================================================
	
	
	UPROPERTY(EditAnywhere)
	float MainWeaponFireRate = .5;

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
	// Weapons system helper variables 
	UPROPERTY(EditAnywhere)
	TSubclassOf<AProjectile> MainWeaponProjectileBlueprint;
	UStaticMeshComponent* FighterMesh;
	bool LeftFiredLast = false;
	float LastFiredTime;
	//====================================================================
	// pysics simulation functions
	void apply_Rotation(float DeltaTime);
	void apply_internal_forces(float DeltaTime);
	

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const & DamageEvent, class AController * EventInstigator, AActor * DamageCauser) override;
	void OnDeath();
	//=====================================================================
	// Input Actions
	void FireMainWeapon();

	//=====================================================================
	//Input Axis Mappings
	void input_SetThrottle(float value);
	void input_SetPitch(float value);
	void input_SetRoll(float value);
	void input_SetYaw(float value);
	//Server Input Replication
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetThrottle(float value);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetPitch(float value);

	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetRoll(float value);
	
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_SetYaw(float value);
	//=====================================================================
	//Input Action Mappings
	void input_Activate_MainWeapon();
	void input_Deactivate_MainWeapon();

	//Replication
	UFUNCTION(Server, Reliable, WithValidation)
	void Server_Toggle_MainWeapon(bool enabled);

	//=====================================================================
	// Debug Helpers
	FString GetEnumText(ENetRole Role);

};
