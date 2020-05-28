

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "StarfighterPlayerController.generated.h"

/**
 * 
 */

enum TeamEnum;

UCLASS()
class MURPHYS_API AStarfighterPlayerController : public APlayerController
{
	GENERATED_BODY()


private:

	UPROPERTY(EditAnywhere)
	float CrosshairXLocation = .5;

	UPROPERTY(EditAnywhere)
	float CrosshairYLocation = .4;

//	UPROPERTY(BlueprintReadWrite)
//	TeamEnum team;

public:

	FVector2D GetSteeringVector();

	//UFUNCTION(Server, Reliable)
	//void Server_TeamSelected(APlayerController* player, TeamEnum team);
	

};
