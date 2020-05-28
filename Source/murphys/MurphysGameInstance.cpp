// Fill out your copyright notice in the Description page of Project Settings.

#include "MurphysGameInstance.h"
#include "OnlineSessionSettings.h"
#include "ARSharedWorldPlayerController.h"
#include "Engine/Engine.h"

#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"
#include "MenuSystem/MainMenu.h"
#include "MoviePlayer.h"
#include "MenuSystem/SLoadingMenuWidget.h"

const static FName SESSION_NAME = TEXT("Game");

// Constructor for the game instance
UMurphysGameInstance::UMurphysGameInstance(const FObjectInitializer& ObjectInitializer) {
	// Get a reference to the main menu class
	ConstructorHelpers::FClassFinder<UUserWidget> MenuBPClass(TEXT("/Game/MenuSystem/WBP_MainMenu"));
	if (!ensure(MenuBPClass.Class != nullptr)) return;
	MenuClass = MenuBPClass.Class;

	// Get a reference to the in game menu class
	ConstructorHelpers::FClassFinder<UUserWidget> ChatWindowBPClass(TEXT("/Game/ChatSystem/WBP_ChatWindow"));
	if (!ensure(ChatWindowBPClass.Class != nullptr)) return;
	ChatWindowClass = ChatWindowBPClass.Class;

	// Initialize In Game Pause Menus
	BindGameMenuReference("MiniGameMenu", TEXT("/Game/MenuSystem/WBP_MinigameMenu"));
	BindGameMenuReference("InGameMenu", TEXT("/Game/MenuSystem/WBP_InGameMenu"));

	// Initialize referances to minigame menu classes
	BindGameMenuReference("StarfighterMenu", TEXT("/Game/MiniGames/Starfighter/StarfighterMenu"));
	
}
//============================================================================
// Game Menu Bindings
//============================================================================
void UMurphysGameInstance::BindGameMenuReference(FName MenuName, const TCHAR *MenuPath)
{
	ConstructorHelpers::FClassFinder<UUserWidget> NamedMenuBPClass(MenuPath);
	if (!ensure(NamedMenuBPClass.Class != nullptr)) return;
	MenuClassMap.Add(MenuName, NamedMenuBPClass.Class);

}

// use this to load minigame menus
bool UMurphysGameInstance::LoadMenuByName(FName MenuName)
{

	//get menu by name
	if (!MenuClassMap.Contains(MenuName)) return false;
	TSubclassOf<class UUserWidget> *NamedMenuClass = MenuClassMap.Find(MenuName);

	// create and load menu
	if (!ensure(NamedMenuClass != nullptr)) return false;
	UUserWidget* NamedMenu = CreateWidget<UUserWidget>(this, *NamedMenuClass);
	if (!ensure(NamedMenu != nullptr)) return false;
	NamedMenu->AddToViewport();

	// Set input mode behaviour
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(NamedMenu->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	APlayerController* PlayerController = GetFirstLocalPlayerController();
	if (!ensure(PlayerController != nullptr)) return false;

	// Setup the input mode and show the cursor for the player
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

	return true;
}

//============================================================================
// Main Menu and Pause Menu Functions
//============================================================================
// Registers the in game menu and opens the panel
void UMurphysGameInstance::LoadInGameMenu()
{
	LoadMenuByName("InGameMenu");
}

void UMurphysGameInstance::LoadMiniGameMenu()
{
	LoadMenuByName("MiniGameMenu");
}

// Registers the main menu
void UMurphysGameInstance::LoadMainMenu() {
	if (!ensure(MenuClass != nullptr)) return;
	
	// Try and create it!
	Menu = CreateWidget<UMainMenu>(this, MenuClass);
	if (!ensure(Menu != nullptr)) return;

	// Remove the peristence manager if we were hosting a game
	if (PersistManager)
	{
		delete PersistManager;
	}

	// Set it up
	Menu->Setup();
	Menu->SetGameInstance(this);

	UnLoadPersistenceManager();
}
//============================================================================
// set up loading screens
//============================================================================

void UMurphysGameInstance::BeginLoadingScreen(const FString &InMapName)
{	
	LoadingMenuWidget = SNew(SLoadingMenuWidget);

	if (!IsRunningDedicatedServer())
	{
		FLoadingScreenAttributes LoadingScreen;
		LoadingScreen.bAutoCompleteWhenLoadingCompletes = false;
		LoadingScreen.WidgetLoadingScreen = SAssignNew(LoadingMenuWidgetContainer, SWeakWidget).PossiblyNullContent(LoadingMenuWidget.ToSharedRef());
		GetMoviePlayer()->SetupLoadingScreen(LoadingScreen);
	}
	
}

void UMurphysGameInstance::EndLoadingScreen(UWorld* InLoadedWorld)
{
	// add additional loading behavior here
	return;
}

// Displays loading menu widget (For Testing Only)
void UMurphysGameInstance::StartLoadingMenuTest()
{
	LoadingMenuWidget = SNew(SLoadingMenuWidget);
	GEngine->GameViewport->AddViewportWidgetContent(SAssignNew(LoadingMenuWidgetContainer, SWeakWidget).PossiblyNullContent(LoadingMenuWidget.ToSharedRef()));
}

// Removes loading Menu widget (For Testing Only)
void UMurphysGameInstance::EndLoadingMenuTest()
{
	if (GEngine && GEngine->GameViewport && LoadingMenuWidgetContainer.IsValid())
	{
		GEngine->GameViewport->RemoveViewportWidgetContent(LoadingMenuWidgetContainer.ToSharedRef());
	}
}

//============================================================================

// Initializes the session game instance
void UMurphysGameInstance::Init() {
	UE_LOG(LogTemp, Warning, TEXT("Game instance initialization"));

	//set up loading screen UMurphysGameInstance
	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &UMurphysGameInstance::BeginLoadingScreen);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &UMurphysGameInstance::EndLoadingScreen);

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

	PersistManager = new PersistenceManager();

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

void UMurphysGameInstance::LoadPersistenceManager()
{
	PersistManager = NewObject<UPersistenceManager>();

	PersistManager->Init();
}

void UMurphysGameInstance::UnLoadPersistenceManager()
{
	if (PersistManager)
	{
		PersistManager->MarkPendingKill();
	}
}

UPersistenceManager* UMurphysGameInstance::GetPersistenceManager()
{
	return PersistManager;
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
	World->ServerTravel("/Game/Lobby/Map/Lobby?listen");
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

