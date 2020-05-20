


#include "MiniGameMenu.h"


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
	GetWorld()->ServerTravel(TEXT("/Game/Lobby/Map/Lobby"));
}