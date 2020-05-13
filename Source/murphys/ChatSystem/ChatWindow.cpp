// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWindow.h"
#include <stack>
#include <map>
#include "UObject/ConstructorHelpers.h"
#include "ARSharedWorldPlayerController.h"
#include "GameFramework/PlayerState.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

const static int MAX_MESSAGES = 40;

UChatWindow::UChatWindow(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	Formats = new MessageFormat(); 

	// Get a reference to the message row's class
	ConstructorHelpers::FClassFinder<UMessageRow> MessageRowBPClass(TEXT("/Game/ChatSystem/WBP_MessageRow"));
	if (!ensure(MessageRowBPClass.Class != nullptr)) return;

	MessageRowClass = MessageRowBPClass.Class;

	// Get a reference to the message row's part class
	ConstructorHelpers::FClassFinder<UMessagePart> MessageRowPartBPClass(TEXT("/Game/ChatSystem/WBP_MessagePart"));
	if (!ensure(MessageRowPartBPClass.Class != nullptr)) return;

	MessagePartClass = MessageRowPartBPClass.Class;
}

// Iniitialize the chat window and add event handlers
bool UChatWindow::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;
	
	return true;
}

// Note: Embedded formats are not currently supported
std::vector<FChatMessagePart> UChatWindow::ParseMessage(FString Message)
{
	// Hold constructed parts
	std::vector<FChatMessagePart> ResultMessageParts;

	// Stack will be used to determine if we are in a format or not
	std::stack<FString> FormatStor;

	// Split message based on beginning formats
	TArray<FString> Parts;
	Message.ParseIntoArray(Parts, TEXT("_:"), true);

	std::vector<FString> AllSplitFormats;

	// Begin splitting each string based on the ending format now
	for (int i = 0; i < Parts.Num(); i++) 
	{
		TArray<FString> AllParts;
		Parts[i].ParseIntoArray(AllParts, TEXT(":_"), true);

		// Push the final split pieces into our vector
		for (int j = 0; j < AllParts.Num(); j++) 
		{
			AllSplitFormats.push_back(AllParts[j]);
		}
	}

	// Loop through each of the parts
	for (int i = 0; i < AllSplitFormats.size(); i++) 
	{
		// Split the string based on the first found space
		FString FullString = AllSplitFormats[i].TrimStartAndEnd();
		uint32 FirstSpace = FullString.Find(TEXT(" "));

		if (FullString.Equals(""))
		{
			continue;
		}

		FString Left = ""; // the format
		FString Right = FullString; // the remaining string

		if (FullString[0] == ':') {
			FullString.Split(" ", &Left, &Right);
		}

		// Valid format, push to stack or pop if not
		if (Formats->IsValidFormat(Left))
		{
			FormatStor.push(Left);
		}
		else if (!FormatStor.empty())
		{
			FormatStor.pop();
		}

		// Create the format struct and begin populating
		FChatMessagePart Part;

		// Support for one word phrases
		if (Right.IsEmpty())
		{
			Right = Left;
		}

		// If we aren't in a format, it'll be default formatting, otherwise push the current format
		if (FormatStor.empty()) {
			Part.Message = Right;
			Part.Formatting = "default";
		} 
		else 
		{
			Part.Message = Right;
			Part.Formatting = FormatStor.top();
		}

		ResultMessageParts.push_back(Part);
	}
	
	// Return a vector of message parts
	return ResultMessageParts;
}

// Called from the blueprint to add a message
// Flow: Blueprint enter & verification -> distribute to all players -> actor function calls addmessage
void UChatWindow::SubmitAddMessage()
{
	// Get the text
	FText Text = MessageEntry->Textbox->Text;

	// Get the world
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	// Get the player's nickname
	ULocalPlayer* Player = GetOwningLocalPlayer();
	if (!ensure(Player != nullptr)) return;

	APlayerController* Controller = Player->GetPlayerController(World);
	if (!ensure(Controller != nullptr)) return;

	APlayerState* State = Controller->PlayerState;
	if (!ensure(State != nullptr)) return;

	FString Nick = State->GetPlayerName();

	// Add the message!
	AddMessage(Nick, Text.ToString());
}

// Add a message based with sender and parsable text
void UChatWindow::AddMessage(FString Sender, FString Message)
{
	if (Message.IsEmpty())
	{
		return;
	}

	// Grab the world
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	// Get the formatted parts
	std::vector<FChatMessagePart> Parts = ParseMessage(Message);

	// Create a row
	UMessageRow* Row = CreateWidget<UMessageRow>(World, MessageRowClass);
	if (!ensure(Row != nullptr)) return;

	// Populate the name on the row
	Row->SetSenderName(Sender);

	// Populate each row part
	for (int i = 0; i < Parts.size(); i++) {
		// Create a new row part
		UMessagePart* RowPart = CreateWidget<UMessagePart>(World, MessagePartClass);
		if (!ensure(RowPart != nullptr)) return;

		// Set the part text and translate the format
		RowPart->SetText(Parts[i].Message);
		Formats->Translate(Parts[i].Formatting, RowPart);

		// Add the part!
		Row->MessageParts->AddChild(RowPart);
	}

	// Remove the first child if we've hit our max messages
	TArray<UWidget*> AllMessages = MessageList->GetAllChildren();
	if (AllMessages.Num() == MAX_MESSAGES)
	{
		AllMessages[0]->SetVisibility(ESlateVisibility::Collapsed);
		AllMessages[0]->RemoveFromParent();
	}

	MessageList->AddChild(Row);
	MessageList->ScrollToEnd();
}

// Toggle the chat as active and inactive
void UChatWindow::ToggleChatActive()
{
	if (IsActive)
	{
		SetChatNotActive();
	} 
	else
	{
		SetChatActive();
	}
}

bool UChatWindow::IsChatActive()
{
	return IsActive;
}

// Set the chat as the active item in the viewport
void UChatWindow::SetChatActive()
{
	Blur->SetVisibility(ESlateVisibility::Visible);
	NewMessageContainer->SetVisibility(ESlateVisibility::Visible);
	MessageList->SetScrollBarVisibility(ESlateVisibility::Visible);

	// TODO: Come up with a better way than looping everytime
	TArray<UWidget*> Children = MessageList->GetAllChildren();
	for (UWidget* Child : Children) {
		Child->SetVisibility(ESlateVisibility::Visible);
	}

	// Set input mode behaviour
	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(MessageEntry->Textbox->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	
	ULocalPlayer* Player = GetOwningLocalPlayer();
	if (!ensure(Player != nullptr)) return;

	// Grab the world
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = Player->GetPlayerController(World);
	if (!ensure(PlayerController != nullptr)) return;

	// Setup the input mode and show the curosr for the player
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = true;

	IsActive = true;
}

// Set the chat as inactive in the viewport
void UChatWindow::SetChatNotActive()
{
	Blur->SetVisibility(ESlateVisibility::Hidden);
	NewMessageContainer->SetVisibility(ESlateVisibility::Hidden);
	MessageList->SetScrollBarVisibility(ESlateVisibility::Collapsed);

	// TODO: Come up with a better way than looping everytime
	TArray<UWidget*> Children = MessageList->GetAllChildren();
	for (UWidget* Child : Children) {
		Child->SetVisibility(ESlateVisibility::Hidden);
	}

	// Set input mode behaviour
	FInputModeGameOnly InputModeData;

	ULocalPlayer* Player = GetOwningLocalPlayer();
	if (!ensure(Player != nullptr)) return;

	// Grab the world
	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	APlayerController* PlayerController = Player->GetPlayerController(World);
	if (!ensure(PlayerController != nullptr)) return;

	// Setup the input mode and show the curosr for the player
	PlayerController->SetInputMode(InputModeData);
	PlayerController->bShowMouseCursor = false;

	IsActive = false;
}