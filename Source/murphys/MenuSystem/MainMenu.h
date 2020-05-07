// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../MurphysGameInstance.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
};


/**
 * 
 */
UCLASS()
class MURPHYS_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()

public: 
	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> ServerNames);

	void SelectIndex(uint32 Index);

	void Setup();
	void Teardown();

	void SetGameInstance(UMurphysGameInstance* Instance);
	
protected:
	virtual bool Initialize();

private:	
	UMurphysGameInstance* GameInstance;
	TSubclassOf<class UUserWidget> ServerRowClass;
	
	// Buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* HostButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuBackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinServerButton;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	/*UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* ServerHostName;

	UPROPERTY(meta = (BindWidget))
	class UButton* CancelHostMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* ConfirmHostMenuButton;*/

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UFUNCTION()
	void HostServer();

	UFUNCTION()
	void JoinServer();

	UFUNCTION()
	void BackToMainMenu();

	UFUNCTION()
	void OpenHostMenu();

	UFUNCTION()
	void OpenJoinMenu();

	UFUNCTION()
	void OpenMainMenu();

	UFUNCTION()
	void QuitPressed();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();
};


