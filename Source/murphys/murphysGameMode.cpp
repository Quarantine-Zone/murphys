// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#include "murphysGameMode.h"
#include "murphysCharacter.h"
#include "UObject/ConstructorHelpers.h"

AmurphysGameMode::AmurphysGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacterSprint"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
