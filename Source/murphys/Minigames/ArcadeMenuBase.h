// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "ArcadeMenuBase.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UArcadeMenuBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	
	virtual bool Initialize();

	UFUNCTION(BlueprintCallable)
	void LoadGame();
	
	//buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;

	UFUNCTION(BlueprintCallable)
	void CloseMenu();

public:

	UPROPERTY(BlueprintReadWrite)
	FString LevelPath;

	UPROPERTY(BlueprintReadWrite)
	FName LevelName;

	
};
