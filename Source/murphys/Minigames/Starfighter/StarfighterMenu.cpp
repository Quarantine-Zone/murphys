// Fill out your copyright notice in the Description page of Project Settings.

#include "Kismet/GameplayStatics.h"
#include "StarfighterMenu.h"


bool UStarfighterMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	//Handle Button Bindings
	
	if (!ensure(CloseButton != nullptr)) return false;
	CloseButton->OnClicked.AddDynamic(this, &UStarfighterMenu::CloseMenu);

	if (!ensure(PlayButton != nullptr)) return false;
	PlayButton->OnClicked.AddDynamic(this, &UStarfighterMenu::LoadGame);

	return true;
}

void UStarfighterMenu::CloseMenu()
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

void UStarfighterMenu::LoadGame()
{
	UE_LOG(LogTemp, Warning, TEXT("LoadGame"));
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("StarfighterLevel_01"));
}