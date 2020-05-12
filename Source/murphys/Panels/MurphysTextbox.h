// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "MurphysTextbox.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UMurphysTextbox : public UUserWidget
{
	GENERATED_BODY()

public: 
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UEditableTextBox* Textbox;
};
