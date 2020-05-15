// Fill out your copyright notice in the Description page of Project Settings.


#include "MessagePart.h"

void UMessagePart::SetText(FString NewText)
{
	Text->SetText(FText::FromString(" " + NewText));
}