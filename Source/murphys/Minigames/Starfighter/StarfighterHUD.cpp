


#include "StarfighterHUD.h"
#include "UObject/ConstructorHelpers.h"


AStarfighterHUD::AStarfighterHUD()
{
	ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetBPClass(TEXT("/Game/ChatSystem/WBP_ChatWindow"));
	if (!ensure(HUDWidgetBPClass.Class != nullptr)) return;
	HUDWidgetClass = HUDWidgetBPClass.Class;
}


void AStarfighterHUD::BeginPlay()
{
	Super::BeginPlay();

	LoadStarfighterHUD();

}
/*
void AStarfighterHUD::UpdateCrosshair(FVector2D pos)
{
	//WBP_StarfighterHUD
	
	//->SetCrosshairPosition();
}
*/

void AStarfighterHUD::LoadStarfighterHUD()
{
	if (!ensure(HUDWidgetClass != nullptr)) return;
	HUDWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
	if (!ensure(HUDWidget != nullptr)) return;
	HUDWidget->AddToViewport();
}

void AStarfighterHUD::CleanHUD() {
	HUDWidget->RemoveFromViewport();
}