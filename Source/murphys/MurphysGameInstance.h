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
	TSubclassOf<class UUserWidget> MinigameMenuClass;
	TSubclassOf<class UUserWidget> StarfighterMenuClass;
	TSubclassOf<class UUserWidget> ChatWindowClass;

	TMap<FName, TSubclassOf<class UUserWidget>> MenuClassMap;

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

	UFUNCTION(BlueprintCallable)
	void LoadInGameMenu();

	UFUNCTION(BlueprintCallable)
	bool LoadMenuByName(FName MenuName); 

	UFUNCTION(BlueprintCallable)
	void LoadMinigameMenu();

	UFUNCTION(Exec)
	void LoadStarfighterMenu();

	void RefreshServerList();
	
	void BindGameMenuReferance(FName MenuName, const TCHAR *MenuPath);

	void Init();
};