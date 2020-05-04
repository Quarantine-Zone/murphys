// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
#include "Public/Interfaces/OnlineSessionInterface.h"
#include "Engine/GameInstance.h"
#include "MurphysGameInstance.generated.h"


/**
 * 
 */
UCLASS()
class MURPHYS_API UMurphysGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionComplete(bool Success);

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	void CreateSession();

	IOnlineSessionPtr SessionInterface;
	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

public:
	void Host();
	void Join(uint32 Index);

	void MurphysGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();
};
