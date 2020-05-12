// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Public/Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "ChatSystem/ChatWindow.h"
#include "MurphysGameInstance.generated.h"

USTRUCT()
struct FServerSettings
{
	GENERATED_BODY()

	FString ServerName;
	uint16 MaxPlayers = 5;
	bool Private = false;
};

/**
 * 
 */
UCLASS()
class MURPHYS_API UMurphysGameInstance : public UGameInstance {
	GENERATED_BODY()
	
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);
	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();
	FServerSettings ServerSettings;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;
	TSubclassOf<class UUserWidget> ChatWindowClass;

	class UMainMenu* Menu;
	class UChatWindow* ChatWindow;

public:
	UMurphysGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(Exec)
	void Host(FServerSettings ServerSettings);

	UFUNCTION(Exec)
	void Join(uint32 Index);

	UFUNCTION(BlueprintCallable)
	void LoadMainMenu();

	UFUNCTION(Exec)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	void LoadChatWindow();

	UFUNCTION(Exec)
	void SetChatActive();

	UFUNCTION(Exec)
	void SetChatNotActive();

	void RefreshServerList();
	
	void Init();
};
