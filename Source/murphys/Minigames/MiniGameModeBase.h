

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MiniGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API AMiniGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	

private:

	bool MatchInProgress; // is there a game in progress?

public:
	
	void PreLogin(APlayerController *NewPlayer) override;

	void PostLogin(APlayerController *NewPlayer) override;

	void Logout(AController *Exiting) override;

	void StartGame();

	void EndGame();

};
