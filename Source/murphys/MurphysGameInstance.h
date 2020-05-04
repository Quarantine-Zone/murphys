// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "OnlineSubsystem.h"
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

	void CreateSession();

	IOnlineSessionPtr SessionInterface;

public:
	void Host();

	void MurphysGameInstance(const FObjectInitializer& ObjectInitializer);
	virtual void Init();
};
