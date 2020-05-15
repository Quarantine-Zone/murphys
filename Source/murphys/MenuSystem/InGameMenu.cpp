// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "../MurphysGameInstance.h"


bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(ResumeButton != nullptr)) return false;
	ResumeButton->Button->OnClicked.AddDynamic(this, &UInGameMenu::ResumeGame);

	if (!ensure(OptionsButton != nullptr)) return false;
	OptionsButton->Button->OnClicked.AddDynamic(this, &UInGameMenu::OpenOptionsMenu);

	if (!ensure(OptionsBackButton != nullptr)) return false;
	OptionsBackButton->Button->OnClicked.AddDynamic(this, &UInGameMenu::BackToInGameMenu);

	if (!ensure(ExitToMainButton != nullptr)) return false;

	return true;
}

void UInGameMenu::BackToInGameMenu()
{
	if (!ensure(InGameMenuSwitcher != nullptr)) return;
	if (!ensure(InGameMenu != nullptr)) return;
	InGameMenuSwitcher->SetActiveWidget(InGameMenu);
}

void UInGameMenu::OpenOptionsMenu()
{
	if (!ensure(InGameMenuSwitcher != nullptr)) return;
	if (!ensure(InGameOptionsMenu != nullptr)) return;
	InGameMenuSwitcher->SetActiveWidget(InGameOptionsMenu);
}

void UInGameMenu::ResumeGame()
{
	if (InGameMenu != nullptr)
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
}