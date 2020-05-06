// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Public/Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
//#include "MenuSystem/MenuInterface.h"
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

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	TSubclassOf<class UUserWidget> MenuClass;
	TSubclassOf<class UUserWidget> InGameMenuClass;

	class UMainMenu* Menu;

public:
	UMurphysGameInstance(const FObjectInitializer& ObjectInitializer);


	//Alex made this to load the menu on game start
	// did not want to f up JT's stuff since it doesn't
	// seem to quite match up with what he was doing.
	UFUNCTION(BlueprintCallable)
	void LoadMenu();

	UFUNCTION(Exec)
	void LoadInGameMenu();

	UFUNCTION(Exec)
	void Host();

	UFUNCTION(Exec)
	void Join(uint32 Index);


	void LoadMenuWidget();

	void InGameLoadMenu();

	void RefreshServerList();

	virtual void LoadMainMenu();
	virtual void Init();
};
