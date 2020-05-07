// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "Components/Slider.h"
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
	class UButton* OpenHostMenuButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* OpenJoinMenuButton;

	// Join menu controls
	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuBackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* JoinMenuExecuteButton;

	UPROPERTY(meta = (BindWidget))
	class UPanelWidget* ServerList;

	// Host menu controls
	UPROPERTY(meta = (BindWidget))
	class UEditableTextBox* HostServerName;

	UPROPERTY(meta = (BindWidget))
	class USlider* HostNumPlayers;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostMenuBackButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* HostMenuExecuteButton;

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


