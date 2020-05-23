


#include "MurphysBasePawn.h"

// Sets default values
AMurphysBasePawn::AMurphysBasePawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMurphysBasePawn::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMurphysBasePawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMurphysBasePawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

