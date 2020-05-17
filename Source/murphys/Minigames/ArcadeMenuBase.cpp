// Fill out your copyright notice in the Description page of Project Settings.


#include "ArcadeMenuBase.h"
#include "Kismet/GameplayStatics.h"

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
	UE_LOG(LogTemp, Warning, TEXT("LoadGame"));

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
	UE_LOG(LogTemp, Warning, TEXT("LoadGame"));
	
	UGameplayStatics::OpenLevel(GetWorld(), LevelName);
}