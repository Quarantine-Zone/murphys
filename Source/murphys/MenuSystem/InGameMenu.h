// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../Panels/MurphysButton.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual bool Initialize();

	// Buttons
	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* ExitToMainButton;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* OptionsBackButton;

	//Menu Switcher
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* InGameMenuSwitcher;
	
	//Switcher Sub-Menus
	UPROPERTY(meta = (BindWidget))
	class UWidget* InGameMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* InGameOptionsMenu;

	// callback funtions
	// Exit to main menu will be handled in the blueprint
	UFUNCTION()
	void BackToInGameMenu();

	UFUNCTION()
	void OpenOptionsMenu();

	UFUNCTION()
	void ResumeGame();

};
