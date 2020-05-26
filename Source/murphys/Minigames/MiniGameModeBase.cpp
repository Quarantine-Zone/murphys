


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


void AMiniGameModeBase::PostLogin(APlayerController *NewPlayer)
{
	Super::PostLogin(NewPlayer);
	//WIP
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
	//WIP
	if (MatchInProgress) {
		// update game status
	}
	else {
		// update teams
	}
}
