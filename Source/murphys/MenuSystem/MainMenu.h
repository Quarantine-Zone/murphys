// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize();

private:	
	// Buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuBackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinServerButton;

	//Menu Switcher
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;
	//Switcher Sub-Menus
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	// callback funtions

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void JoinServer();

};
