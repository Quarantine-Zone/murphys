// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeMenuBase.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "../MurphysGameInstance.h"

bool UArcadeMenuBase::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	//Handle Button Bindings

	if (!ensure(CloseButton != nullptr)) return false;
	CloseButton->OnClicked.AddDynamic(this, &UArcadeMenuBase::CloseMenu);

	if (!ensure(PlayButton != nullptr)) return false;
	PlayButton->OnClicked.AddDynamic(this, &UArcadeMenuBase::LoadGame);

	return true;
}

void UArcadeMenuBase::CloseMenu()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->SetPause(false);

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;

}

void UArcadeMenuBase::LoadGame()
{	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	CloseMenu();
	if (World->GetFirstPlayerController()->HasAuthority())
	{
		//UMurphysGameInstance *MyGI = (UMurphysGameInstance *) GetGameInstance();
		//MyGI->BeginLoadingScreen();
		World->ServerTravel(LevelPath);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("cannot choose gamemode when not host"));
	}
	

}