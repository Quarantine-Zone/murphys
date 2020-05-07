// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Public/Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "MurphysGameInstance.generated.h"

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
	FString DesiredServerName;

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

public:
	UMurphysGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(Exec)
	void Host(FString ServerName);

	UFUNCTION(Exec)
	void Join(uint32 Index);

	UFUNCTION(BlueprintCallable)
	void LoadMenuWidget();

	UFUNCTION(Exec)
	void LoadInGameMenu();

	void RefreshServerList();

	void StartSession();
	
	void LoadMainMenu();

	void Init();
};
