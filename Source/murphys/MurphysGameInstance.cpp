// Fill out your copyright notice in the Description page of Project Settings.


#include "MurphysGameInstance.h"
#include "OnlineSessionSettings.h"
#include "Public/Interfaces/OnlineSessionInterface.h"
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

