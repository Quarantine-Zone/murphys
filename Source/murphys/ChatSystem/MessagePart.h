// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "MessagePart.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UMessagePart : public UUserWidget
{
	GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
    class UTextBlock* Text;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    bool UseRainbow = false;

    void SetText(FString NewText);
};
