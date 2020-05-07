// Fill out your copyright notice in the Description page of Project Settings.

#include "MurphysGameInstance.h"
#include "OnlineSessionSettings.h"
#include "ARSharedWorldPlayerController.h"
#include "Engine/Engine.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"

const static FName SESSION_NAME = TEXT("Game");
const static FName SERVER_NAME_SETTINGS_KEY = TEXT("ServerName");

UMurphysGameInstance::UMurphysGameInstance(const FObjectInitializer& ObjectInitializer) {
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

void UMurphysGameInstance::LoadInGameMenu()
{
	if (!ensure(InGameMenuClass != nullptr)) return;
	UUserWidget* InGameMenu = CreateWidget<UUserWidget>(this, InGameMenuClass);
	if (!ensure(InGameMenu != nullptr)) return;
	InGameMenu->AddToViewport();

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(InGameMenu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = true;
}

void UMurphysGameInstance::LoadMenuWidget() {
	if (!ensure(MenuClass != nullptr)) return;
	
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	Menu->Setup();

	Menu->SetGameInstance(this);
}

void UMurphysGameInstance::LoadMainMenu() {
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	PlayerController->ClientTravel("/Game/MenuSystem/MainMenu", ETravelType::TRAVEL_Absolute);


}

void UMurphysGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("Game instance initialization"));

	// Get the online subsystem
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	if (subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *subsystem->GetSubsystemName().ToString());
		
		// Get the session interface
		SessionInterface = subsystem->GetSessionInterface();
		if (SessionInterface.IsValid()) {
			SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &UMurphysGameInstance::OnCreateSessionComplete);
			SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &UMurphysGameInstance::OnDestroySessionComplete);
			SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &UMurphysGameInstance::OnJoinSessionComplete);
			SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &UMurphysGameInstance::OnFindSessionComplete);
		}
	}
}

// Should be called via the main menu to create a new session
void UMurphysGameInstance::Host(FString ServerName) {
	DesiredServerName = ServerName;
	if (!SessionInterface.IsValid()) {
		return;
	}

	// Create a new session if we don't have one, otherwise cleanup and create it
	auto ExistingSession = SessionInterface->GetNamedSession(SESSION_NAME);
	if (ExistingSession != nullptr) {
		SessionInterface->DestroySession(SESSION_NAME);
	} else {
		CreateSession(); 
	}
}

// Called to actually create a session
void UMurphysGameInstance::CreateSession() {
	if (!SessionInterface.IsValid()) {
		return;
	}

	FOnlineSessionSettings SessionSettings;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
	}
	SessionSettings.NumPublicConnections = 5;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(SERVER_NAME_SETTINGS_KEY, DesiredServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

void UMurphysGameInstance::Join(uint32 Index) {
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) {
		return;
	}

	if (Menu != nullptr) {
		Menu->Teardown();
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

void UMurphysGameInstance::RefreshServerList() {
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting to find sessions..."));
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

// Delegate callback once a session has been destroyed
void UMurphysGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}
}

void UMurphysGameInstance::StartSession() {
	if (SessionInterface.IsValid())
	{
		SessionInterface->StartSession(SESSION_NAME);
	}
}

// Delegate callback once a session has been created
void UMurphysGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) {
		return;
	}

	if (Menu != nullptr) {
		Menu->Teardown();
	}

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) {
		return;
	}

	// TODO: Update with lobby name
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/Lobby?listen");
}

void UMurphysGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (!SessionInterface.IsValid()) {
		return;
	}

	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) {
		return;
	}

	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Joining %s"), *Address);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) {
		return;
	}

	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UMurphysGameInstance::OnFindSessionComplete(bool Success) {
	if (!Success || !SessionSearch.IsValid() && Menu != nullptr) {
		return;
	}

	TArray<FServerData> ServerNames;
	UE_LOG(LogTemp, Warning, TEXT("Find sessions completed"));
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults) {
		UE_LOG(LogTemp, Warning, TEXT("Found session named %s"), *SearchResult.GetSessionIdStr());		
		
		FServerData Data;
		Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
		Data.HostUsername = SearchResult.Session.OwningUserName;
		FString ServerName;

		if (SearchResult.Session.SessionSettings.Get(SERVER_NAME_SETTINGS_KEY, ServerName))
		{
			Data.Name = ServerName;
		}
		else
		{
			Data.Name = "Could not find name.";
		}
		ServerNames.Add(Data);
	}

	Menu->SetServerList(ServerNames);
}