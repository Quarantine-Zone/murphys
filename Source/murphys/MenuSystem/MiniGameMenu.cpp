


#include "MiniGameMenu.h"
#include "Engine/Engine.h"
#include "../Minigames/Starfighter/StarfighterHUD.h"



bool UMiniGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(ExitToLobbyButton != nullptr)) return false;
	ExitToLobbyButton->Button->OnClicked.AddDynamic(this, &UMiniGameMenu::ExitToLobby);

	return true;
}

	
void UMiniGameMenu::ExitToLobby()
{
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) {
		return;
	}

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	
	//AStarfighterHUD* SF_HUD =  (AStarfighterHUD*) PlayerController->GetHUD();
	//SF_HUD->CleanHUD();

	PlayerController->SetPause(false);

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;

	// Send the player to the lobby
	ResumeGame();
	World->ServerTravel("/Game/Lobby/Map/Lobby");
}