// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>

#include "Blueprint/UserWidget.h"
#include "../Panels/MurphysTextbox.h"
#include "../Panels/MurphysButton.h"
#include "Components/ScrollBox.h"
#include "Components/SizeBox.h"
#include "Components/BackgroundBlur.h"
#include "MessageRow.h"
#include "MessagePart.h"
#include "MessageFormat.h"
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
	TSubclassOf<class UUserWidget> MessageRowClass;
	TSubclassOf<class UUserWidget> MessagePartClass;
	MessageFormat* Formats;

public: 
	UChatWindow(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool IsActive = false;

	bool Initialize();

	UFUNCTION(BlueprintCallable)
	void SubmitAddMessage();

	UFUNCTION(BlueprintCallable)
	void AddMessage(FString Sender, FString Message);

	UFUNCTION(BlueprintCallable)
	void SetChatActive();

	UFUNCTION(BlueprintCallable)
	void SetChatNotActive();

	UFUNCTION(BlueprintCallable)
	bool IsChatActive();

	UFUNCTION(BlueprintCallable)
	void ToggleChatActive();

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UMurphysTextbox* MessageEntry;

	UPROPERTY(meta = (BindWidget))
	class UMurphysButton* Submit;

	UPROPERTY(meta = (BindWidget))
	class UScrollBox* MessageList;

	UPROPERTY(meta = (BindWidget))
	class USizeBox* NewMessageContainer;

	UPROPERTY(meta = (BindWidget))
	class UBackgroundBlur* Blur;
};
