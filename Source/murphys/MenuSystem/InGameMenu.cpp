// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"


bool UInGameMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(ResumeButton != nullptr)) return false;
	ResumeButton->OnClicked.AddDynamic(this, &UInGameMenu::ResumeGame);

	if (!ensure(OptionsButton != nullptr)) return false;
	OptionsButton->OnClicked.AddDynamic(this, &UInGameMenu::OpenOptionsMenu);

	if (!ensure(OptionsBackButton != nullptr)) return false;
	OptionsBackButton->OnClicked.AddDynamic(this, &UInGameMenu::BackToInGameMenu);

	if (!ensure(ExitToMainButton != nullptr)) return false;
	ExitToMainButton->OnClicked.AddDynamic(this, &UInGameMenu::ExitToMain);

	return true;
}

void UInGameMenu::ExitToMain()
{
	UE_LOG(LogTemp, Warning, TEXT("I don't work yet"));
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

}