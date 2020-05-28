

#pragma once

#include "CoreMinimal.h"
#include "InGameMenu.h"
#include "MiniGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UMiniGameMenu : public UInGameMenu
{
	GENERATED_BODY()

protected:
	virtual bool Initialize();

public:

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* ExitToLobbyButton;

	UFUNCTION(BlueprintCallable)
	void ExitToLobby();
};
