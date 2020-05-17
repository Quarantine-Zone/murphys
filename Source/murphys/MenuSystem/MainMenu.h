// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "../MurphysGameInstance.h"
#include "../Panels/MurphysButton.h"
#include "../Panels/MurphysTextbox.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
	int32 Ping;
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
	
	UPROPERTY(meta = (BindWidget))
	class UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidget* HostMenu;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* MenuSwitcher;

	// Main menu buttons
	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* OpenHostMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* OpenJoinMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* QuitGameButton;

	// Join menu controls
	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* JoinMenuBackButton;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* JoinMenuExecuteButton;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* JoinMenuResultText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* JoinMenuLoadingText;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* JoinMenuRefreshButton;

	// Host menu controls
	UPROPERTY(meta = (BindWidget))
	class UMurphysTextbox* HostServerName;

	UPROPERTY(meta = (BindWidget))
	class USlider* HostNumPlayers;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* HostMenuBackButton;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* HostMenuExecuteButton;

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

	UFUNCTION()
	void RefreshServers();

	TOptional<uint32> SelectedIndex;

	void UpdateChildren();
};


