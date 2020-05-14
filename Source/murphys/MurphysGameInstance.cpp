// Fill out your copyright notice in the Description page of Project Settings.

#include "MurphysGameInstance.h"
#include "OnlineSessionSettings.h"
#include "ARSharedWorldPlayerController.h"
#include "Engine/Engine.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"

const static FName SESSION_NAME = TEXT("Game");

// Constructor for the game instance
UMurphysGameInstance::UMurphysGameInstance(const FObjectInitializer& ObjectInitializer) {
	// Get a reference to the main menu class
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;

	MenuClass = MenuBPClass.Class;

	// Get a reference to the in game menu class
	ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBPClass(TEXT("/Game/MenuSystem/WBP_InGameMenu"));
	if (!ensure(InGameMenuBPClass.Class != nullptr)) return;

	InGameMenuClass = InGameMenuBPClass.Class;
}

// Registers the in game menu and opens the panel
void UMurphysGameInstance::LoadInGameMenu()
{
	// Try and create the menu and add it to the viewport
	if (!ensure(InGameMenuClass != nullptr)) return;
	UUserWidget* InGameMenu = CreateWidget<UUserWidget>(this, InGameMenuClass);
	
	if (!ensure(InGameMenu != nullptr)) return;
	InGameMenu->AddToViewport();

	// Set input mode behaviour
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(InGameMenu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	// Setup the input mode and show the curosr for the player
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

// Registers the main menu
void UMurphysGameInstance::LoadMainMenu() {
	if (!ensure(MenuClass != nullptr)) return;
	
	// Try and create it!
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	// Set it up
	Menu->Setup();
	Menu->SetGameInstance(this);
}

// Initializes the session game instance
void UMurphysGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("Game instance initialization"));

	// Get the online subsystem
	IOnlineSubsystem* subsystem = IOnlineSubsystem::Get();
	if (subsystem != nullptr) {
		UE_LOG(LogTemp, Warning, TEXT("Found subsystem %s"), *subsystem->GetSubsystemName().ToString());
		
		// Get the session interface and register delegates
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
void UMurphysGameInstance::Host(FServerSettings Settings) {
	// Hold a reference to the settings
	ServerSettings = Settings;
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
	// Make sure the session is valid
	if (!SessionInterface.IsValid()) {
		return;
	}

	// Construct the session settings based on those provided by the user
	FOnlineSessionSettings SessionSettings;
	if (IOnlineSubsystem::Get()->GetSubsystemName() == "NULL")
	{
		SessionSettings.bIsLANMatch = true;
		UE_LOG(LogTemp, Warning, TEXT("Starting a local session..."));
	}
	else
	{
		SessionSettings.bIsLANMatch = false;
		UE_LOG(LogTemp, Warning, TEXT("Starting a steam session..."));
	} 

	if (ServerSettings.ServerName.IsEmpty()) {
		UE_LOG(LogTemp, Warning, TEXT("Server name not set, cancelling session creation"));
		return;
	}

	// Setup some more settings
	SessionSettings.NumPublicConnections = ServerSettings.MaxPlayers;
	SessionSettings.bShouldAdvertise = !ServerSettings.Private;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bAllowJoinInProgress = true;
	SessionSettings.Set("ServerName", ServerSettings.ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	// Create the session
	SessionInterface->CreateSession(0, SESSION_NAME, SessionSettings);
}

// Attempt to join a server based on a result index
void UMurphysGameInstance::Join(uint32 Index) {
	if (!SessionInterface.IsValid() || !SessionSearch.IsValid()) {
		return;
	}

	// Destroy the menu
	if (Menu != nullptr) {
		Menu->Teardown();
	}

	// Do the join!
	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
}

// Refresh the servers on the main menu
void UMurphysGameInstance::RefreshServerList() {
	// Start a new search
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid()) {
		
		// Setup some search parameters
		SessionSearch->MaxSearchResults = 10000000;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		UE_LOG(LogTemp, Warning, TEXT("Starting to find sessions..."));

		// Find the sessions!
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

// Delegate callback once a session has been destroyed
void UMurphysGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}
}

// Delegate once a session has been created
void UMurphysGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {
	if (!Success) {
		UE_LOG(LogTemp, Warning, TEXT("Could not create session"));
		return;
	}

	// Get the engine
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) {
		return;
	}

	// Destroy the menu
	if (Menu != nullptr) {
		Menu->Teardown();
	}

	// Let the user know they're hosting
	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));

	// Get the world
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) {
		return;
	}

	// Send the player to the lobby
	World->ServerTravel("/Game/ThirdPersonCPP/Maps/Lobby?listen");
}

// Delegate once a user tries to join a session
void UMurphysGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result) {
	if (!SessionInterface.IsValid()) {
		return;
	}

	// Get the engine
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) {
		return;
	}

	// Get the connection address
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address)) {
		UE_LOG(LogTemp, Warning, TEXT("Could not get connect string"));
		return;
	}

	// Get the target player
	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) {
		return;
	}

	// Send the player!
	PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

// Delegate once find sessions has resolved
void UMurphysGameInstance::OnFindSessionComplete(bool Success) {
	if (!Success || !SessionSearch.IsValid() && Menu != nullptr) {
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Find sessions completed"));

	// Loop through the found sessions
	TArray<FServerData> ServerNames;
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults) {
		UE_LOG(LogTemp, Warning, TEXT("Found session named %s"), *SearchResult.GetSessionIdStr());		
		
		// Construct a server data object, which will be used to populate the row
		FServerData Data;
		Data.Ping = SearchResult.PingInMs;
		Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
		Data.HostUsername = SearchResult.Session.OwningUserName;
		FString ServerName;

		// Determine if we have a set name
		if (SearchResult.Session.SessionSettings.Get("ServerName", ServerName))
		{
			Data.Name = ServerName;
		}
		else
		{
			continue;
		}

		// Add the data 
		ServerNames.Add(Data);
	}

	// Set the main menu's server list
	Menu->SetServerList(ServerNames);
}