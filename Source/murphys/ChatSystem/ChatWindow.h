// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "vector"
#include "Blueprint/UserWidget.h"
#include "../Panels/MurphysTextbox.h"
#include "../Panels/MurphysButton.h"
#include "ChatWindow.generated.h"

USTRUCT()
struct FChatMessagePart
{
	GENERATED_BODY()

	FString Message;
	FString Formatting;
};

/**
 * 
 */
UCLASS()
class MURPHYS_API UChatWindow : public UUserWidget
{
	GENERATED_BODY()

	std::vector<FChatMessagePart> ParseMessage(FString Message);

public: 
	UChatWindow(const FObjectInitializer& ObjectInitializer);

	bool Initialize();

	void addMessage(FText Message);

	
	UFUNCTION()
	void SubmitMessage();

	UFUNCTION()
	void TextCommitted(const FText& InText, ETextCommit::Type InCommitType);

	UPROPERTY(meta = (BindWidget))
	class UMurphysTextbox* MessageEntry;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* Submit;
};
