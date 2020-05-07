// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "ServerRow.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API UServerRow : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ServerName;

	/*UPROPERTY(meta = (BindWidget))
	class UTextBlock* HostUser;*/

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* PlayerCount;

	UPROPERTY(BlueprintReadOnly)
	bool Selected = false;

	void Setup(class UMainMenu* Parent, uint32 Index);

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;

	UPROPERTY()
		class UMainMenu* Parent;

	uint32 Index;

	UFUNCTION()
		void OnClicked();
};
