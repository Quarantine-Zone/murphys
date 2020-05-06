// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

private:
	// Buttons
	UPROPERTY(meta = (BindWidget))
		class UButton* ResumeButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* OptionsButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* ExitToMainButton;

	UPROPERTY(meta = (BindWidget))
		class UButton* OptionsBackButton;

	//Menu Switcher
	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* InGameMenuSwitcher;
	//Switcher Sub-Menus
	UPROPERTY(meta = (BindWidget))
		class UWidget* InGameMenu;

	UPROPERTY(meta = (BindWidget))
		class UWidget* InGameOptionsMenu;

	// callback funtions

	UFUNCTION()
		void ExitToMain();

	UFUNCTION()
		void BackToInGameMenu();

	UFUNCTION()
		void OpenOptionsMenu();

	UFUNCTION()
		void ResumeGame();

};
