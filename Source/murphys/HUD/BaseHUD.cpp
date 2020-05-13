// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseHUD.h"

UBaseHUD::UBaseHUD(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Added base HUD to screen"));
}