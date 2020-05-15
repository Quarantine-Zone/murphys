
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "Components/WrapBox.h"
#include "MessageRow.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UMessageRow : public UUserWidget
{
	GENERATED_BODY()

public: 
    UMessageRow(const FObjectInitializer& ObjectInitializer);

    void SetSenderName(FString Nick);

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* MessageSender;

    UPROPERTY(meta = (BindWidget))
    class UWrapBox* MessageParts;
};
