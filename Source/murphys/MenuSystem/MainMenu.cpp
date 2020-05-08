// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"

#include "UObject/ConstructorHelpers.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

#include "ServerRow.h"

// Menu constructor
UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	// Get the class for the server row
	ConstructorHelpers::FClassFinder<UServerRow> ServerRowBPClass(TEXT("/Game/MenuSystem/WBP_ServerRow"));
	if (!ensure(ServerRowBPClass.Class != nullptr)) return;

	ServerRowClass = ServerRowBPClass.Class;
}

// Called once the menu has initialized
bool UMainMenu::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	// Main menu bindings
	if (!ensure(OpenHostMenuButton != nullptr)) return false;
	OpenHostMenuButton->Button->OnClicked.AddDynamic(this, &UMainMenu::OpenHostMenu);

	if (!ensure(OpenJoinMenuButton != nullptr)) return false;
	OpenJoinMenuButton->Button->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);

	if (!ensure(QuitGameButton != nullptr)) return false;
	QuitGameButton->Button->OnClicked.AddDynamic(this, &UMainMenu::QuitPressed);

	// Join menu bindings
	if (!ensure(JoinMenuBackButton != nullptr)) return false;
	JoinMenuBackButton->Button->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	if (!ensure(JoinMenuExecuteButton != nullptr)) return false;
	JoinMenuExecuteButton->Button->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	// Host menu bindings
	if (!ensure(HostMenuBackButton != nullptr)) return false;
	HostMenuBackButton->Button->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	if (!ensure(HostMenuExecuteButton != nullptr)) return false;
	HostMenuExecuteButton->Button->OnClicked.AddDynamic(this, &UMainMenu::HostServer);

	return true;
}

// Sets a reference to the game instance
void UMainMenu::SetGameInstance(UMurphysGameInstance* Instance)
{
	GameInstance = Instance;
}

// Update the server row list with the found search results
void UMainMenu::SetServerList(TArray<FServerData> ServerNames)
{
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	// Clear children
	ServerList->ClearChildren();

	uint32 i = 0;
	for (const FServerData& ServerData : ServerNames)
	{
		// Create a new row
		UServerRow* Row = CreateWidget<UServerRow>(World, ServerRowClass);
		if (!ensure(Row != nullptr)) return;

		// Text for NumPlayers/MaxPlayers
		FString FractionText = FString::Printf(TEXT("%d/%d"), ServerData.CurrentPlayers, ServerData.MaxPlayers);
		
		// Set the text
		Row->ServerName->SetText(FText::FromString(ServerData.Name));
		Row->PlayerCount->SetText(FText::FromString(FractionText));
		Row->Setup(this, i);

		i++;

		// Add the row
		ServerList->AddChild(Row);
	}
}

// Once a row is clicked, update it as selected and update each row
void UMainMenu::SelectIndex(uint32 Index)
{
	SelectedIndex = Index;
	UpdateChildren();
}

// Sets a variable on each row equal to if it's selected
void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerList->GetChildrenCount(); ++i)
	{
		// Get the row
		auto Row = Cast<UServerRow>(ServerList->GetChildAt(i));
		if (Row != nullptr)
		{
			// Set selected
			Row->Selected = (SelectedIndex.IsSet() && SelectedIndex.GetValue() == i);
		}
	}
}

// 
void UMainMenu::OpenHostMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(HostMenu != nullptr)) return;
	MenuSwitcher->SetActiveWidget(HostMenu);
}

void UMainMenu::HostServer()
{
	if (GameInstance != nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("I'm gonna Wreck it!"));
		FServerSettings ServerSettings;
		ServerSettings.ServerName = HostServerName->Text.ToString();
		ServerSettings.MaxPlayers = HostNumPlayers->GetValue();

		if (ServerSettings.ServerName.IsEmpty()) {
			return;
		}

		GameInstance->Host(ServerSettings);
	}
}

// Called once the user wants to travel from the host/join menu -> main menu
void UMainMenu::BackToMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(MainMenu != nullptr)) return;

	// Activate it
	MenuSwitcher->SetActiveWidget(MainMenu);
	UE_LOG(LogTemp, Warning, TEXT("JT is a Badass"));
}

// Called from the main menu to switch to the join menu
void UMainMenu::OpenJoinMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	// Activate it and refresh the servers
	MenuSwitcher->SetActiveWidget(JoinMenu);
	if (GameInstance != nullptr) {
		GameInstance->RefreshServerList();
	}

	UE_LOG(LogTemp, Warning, TEXT("Ethan is ok I guess"));
}

// Called once a user would like to join a selected server
void UMainMenu::JoinServer()
{
	if (SelectedIndex.IsSet() && GameInstance != nullptr)
	{
		// Call the game instance to join
		GameInstance->Join(SelectedIndex.GetValue());
	}
}

// Called once the quit button has been pressed
void UMainMenu::QuitPressed()
{
	// Get the world
	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	// Get the player
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	// Make the player quit!
	PlayerController->ConsoleCommand("quit");
}

// Called to open the main menu
void UMainMenu::OpenMainMenu()
{
	if (!ensure(MenuSwitcher != nullptr)) return;
	if (!ensure(JoinMenu != nullptr)) return;

	// Activate it!
	MenuSwitcher->SetActiveWidget(MainMenu);
}

// Called to setup the main menu
void UMainMenu::Setup()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;
}

// Called to destroy the main menu
void UMainMenu::Teardown()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!ensure(PlayerController != nullptr)) return;

	FInputModeGameOnly InputModeData;
	PlayerController->SetInputMode(InputModeData);

	PlayerController->bShowMouseCursor = false;
}