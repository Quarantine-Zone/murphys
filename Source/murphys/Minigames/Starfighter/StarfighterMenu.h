// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "StarfighterMenu.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UStarfighterMenu : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual bool Initialize();

private:
	
	//buttons
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	class UButton* PlayButton;


	UFUNCTION(BlueprintCallable)
	void CloseMenu();

	UFUNCTION(BlueprintCallable)
	void LoadGame();



};
