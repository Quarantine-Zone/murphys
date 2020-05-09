// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MurphysButton.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UMurphysButton : public UUserWidget
{
	GENERATED_BODY()
	
public: 
	UPROPERTY(meta = (BindWidget))
	class UButton* Button;

};
