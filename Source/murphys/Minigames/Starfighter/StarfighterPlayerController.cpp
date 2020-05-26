


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

	//float a = 1;
	//float b = SteeringVector.Distance;
	//float c = 1 + pow(b, 2);
	
	//float turnAngle = atan(b);
	//float pitch = ;
	//float yaw
	
	

	UE_LOG(LogTemp, Warning, TEXT("ScreenLocation: X= %f, y=%f"), ScreenLocation.X, ScreenLocation.Y);
	UE_LOG(LogTemp, Warning, TEXT("mouse: X= %f, y=%f"), MouseX, MouseY);

	return SteeringVector;

	/*
	FVector CameraWorldLocation; // To be discarded
	DeprojectScreenPositionToWorld(
		ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection);
	*/
}

/*
void AStarfighterPlayerController::DrawJoyLine
(
	const FVector2D& Start,
	const FVector2D& End,
	const FLinearColor& TheColor,
	const float& Thick
)
{
	if (!Canvas) return;
	//

	FCanvasLineItem NewLine(Start, End);
	NewLine.SetColor(TheColor);
	NewLine.LineThickness = Thick;
	Canvas->DrawItem(NewLine);
*/