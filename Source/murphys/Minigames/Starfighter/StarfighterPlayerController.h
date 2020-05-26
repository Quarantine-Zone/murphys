

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StarfighterPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API AStarfighterPlayerController : public APlayerController
{
	GENERATED_BODY()


private:

	UPROPERTY(EditAnywhere)
	float CrosshairXLocation = .5;

	UPROPERTY(EditAnywhere)
	float CrosshairYLocation = .4;

public:

	FVector2D GetSteeringVector();

};
