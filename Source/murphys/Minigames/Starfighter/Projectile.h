

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class MURPHYS_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UProjectileMovementComponent* ProjectileMovement = nullptr;
	//UStaticMeshComponent* CollisionMesh = nullptr;
	//UParticleSystemComponent* LaunchBlast = nullptr; 

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void LaunchProjectile();
	void onHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent);
};
