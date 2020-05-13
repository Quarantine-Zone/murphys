// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "../ChatSystem/ChatWindow.h"
#include "BaseHUD.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UBaseHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UBaseHUD(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UChatWindow* ChatWindow;
};
