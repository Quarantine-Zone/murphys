


#include "MiniGameModeBase.h"

//==============================================================
// game state handling
//==============================================================
// call this at the start of a match
void AMiniGameModeBase::StartGame() 
{
	MatchInProgress = true;
}

// call this at the end of a match
void AMiniGameModeBase::EndGame() 
{
	MatchInProgress = false;
}

//=============================================================
// player session handling
//=============================================================


void AMiniGameModeBase::PreLogin(APlayerController *NewPlayer)
{
	//Super::PreLogin();
}

void AMiniGameModeBase::PostLogin(APlayerController *NewPlayer)
{
	Super::PostLogin(NewPlayer);

	if (MatchInProgress) {
		// join as spectator
		//BecomeSpectator(NewPlayer);
	} else {
		// join as player
		//BecomePlayer(NewPlayer);
	}
}

void AMiniGameModeBase::Logout(AController *Exiting)
{
	Super::Logout(Exiting);
}
