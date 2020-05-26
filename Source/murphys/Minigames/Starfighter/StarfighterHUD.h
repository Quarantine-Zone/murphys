

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "Blueprint/UserWidget.h"
#include "StarfighterHUD.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API AStarfighterHUD : public AHUD
{
	GENERATED_BODY()
	

private:
	TSubclassOf<class UUserWidget> HUDWidgetClass;
	UUserWidget* HUDWidget;

public:
	AStarfighterHUD();

	void  BeginPlay() override;
	//void  Tick(float DeltaTime) override;

	void CleanHUD();
	void LoadStarfighterHUD();

	//void UpdateCrosshair(FVector2D pos);

};