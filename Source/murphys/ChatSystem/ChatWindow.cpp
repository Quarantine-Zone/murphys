// Fill out your copyright notice in the Description page of Project Settings.


#include "ChatWindow.h"

#include "UObject/ConstructorHelpers.h"
#include "stack"
#include "map"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"

UChatWindow::UChatWindow(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

bool UChatWindow::Initialize()
{
	bool Success = Super::Initialize();
	if (!Success) return false;

	if (!ensure(MessageEntry != nullptr)) return false;
	MessageEntry->Textbox->OnTextCommitted.AddDynamic(this, &UChatWindow::TextCommitted);

	if (!ensure(Submit != nullptr)) return false;
	Submit->Button->OnClicked.AddDynamic(this, &UChatWindow::SubmitMessage);
	
	return true;
}

// Note: Embedded formats are not currently supported
std::vector<FChatMessagePart> UChatWindow::ParseMessage(FString Message)
{
	std::vector<FChatMessagePart> ResultMessageParts;
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

	std::map<FString, FString> ValidFormats;
	ValidFormats[":red"] = "Red";
	ValidFormats[":green"] = "Green";
	ValidFormats[":yellow"] = "Yellow";
	ValidFormats[":default"] = "Default";

	for (int i = 0; i < AllSplitFormats.size(); i++) 
	{
		FString FullString = AllSplitFormats[i].TrimStartAndEnd();
		uint32 FirstSpace = FullString.Find(TEXT(" "));

		FString Left = "";
		FString Right = FullString;

		if (FullString[0] == ':') {
			FullString.Split(" ", &Left, &Right);
		}

		// Valid format, push to stackm or pop if not
		if (ValidFormats.find(Left) != ValidFormats.end()) 
		{
			FormatStor.push(Left);
		}
		else if (!FormatStor.empty())
		{
			FormatStor.pop();
		}

		// Create the format struct and push
		FChatMessagePart Part;

		// Support for one word phrases
		if (Right.IsEmpty())
		{
			Right = Left;
		}

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
	
	return ResultMessageParts;
}

void UChatWindow::addMessage(FText Message)
{
	std::vector<FChatMessagePart> Parts = ParseMessage(Message.ToString());
}

void UChatWindow::TextCommitted(const FText& InText, ETextCommit::Type InCommitType)
{
	if (InCommitType != ETextCommit::OnEnter) return;

	addMessage(InText);
}

void UChatWindow::SubmitMessage()
{
	FText Text = MessageEntry->Textbox->Text;
	addMessage(Text);

	return;
}