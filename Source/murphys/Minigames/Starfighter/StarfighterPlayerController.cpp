


#include "StarfighterPlayerController.h"


FVector2D AStarfighterPlayerController::GetSteeringVector()
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);
	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrosshairXLocation, ViewportSizeY * CrosshairYLocation);

	float MouseX;
	float MouseY;

	GetMousePosition(MouseX, MouseY);
	FVector2D SteeringVector = ScreenLocation - FVector2D(MouseX, MouseY);

	return SteeringVector;

}