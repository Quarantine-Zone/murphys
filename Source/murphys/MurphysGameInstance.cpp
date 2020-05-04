// Fill out your copyright notice in the Description page of Project Settings.


#include "MurphysGameInstance.h"
#include "OnlineSessionSettings.h"
#include "ARSharedWorldPlayerController.h"
#include "Engine/Engine.h"

const static FName SESSION_NAME = TEXT("Test Session");

void UMurphysGameInstance::MurphysGameInstance(const FObjectInitializer& ObjectInitializer) {
	UE_LOG(LogTemp, Warning, TEXT("Game instance constructor"));
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

			SessionSearch = MakeShareable(new FOnlineSessionSearch());
			if (SessionSearch.IsValid()) {
				//SessionSearch->bIsLanQuery = true;
				SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
			}
		}
	}
}

// Should be called via the main menu to create a new session
void UMurphysGameInstance::Host() {
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

	// Create a new session
	FOnlineSessionSettings settings;
	settings.bIsLANMatch = true;
	settings.NumPublicConnections = 2;
	settings.bShouldAdvertise = true;
	SessionInterface->CreateSession(0, SESSION_NAME, settings);
}

// Delegate callback once a session has been destroyed
void UMurphysGameInstance::OnDestroySessionComplete(FName SessionName, bool Success) {
	if (Success) {
		CreateSession();
	}
}

// Delegate callback once a session has been created
void UMurphysGameInstance::OnCreateSessionComplete(FName SessionName, bool Success) {
	UEngine* Engine = GetEngine();
	if (!ensure(Engine != nullptr)) {
		return;
	}

	Engine->AddOnScreenDebugMessage(0, 2, FColor::Green, TEXT("Hosting"));
	
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) {
		return;
	}

	World->ServerTravel("/Game/ThirdPersonCPP/Maps/ThirdPersonExampleMap?listen");
}

void UMurphysGameInstance::Join(uint32 Index) {
	if (!SessionInterface.IsValid() || SessionSearch.IsValid()) {
		return;
	}

	SessionInterface->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);
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
	if (!Success || !SessionSearch.IsValid()) {
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Find sessions completed"));
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults) {
		UE_LOG(LogTemp, Warning, TEXT("Found session named %s"), *SearchResult.GetSessionIdStr());
	}
}