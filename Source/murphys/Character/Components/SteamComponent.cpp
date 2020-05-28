


#include "SteamComponent.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerState.h"
#include "ThirdParty/Steamworks/Steamv146/sdk/public/steam/steam_api.h"
#include <string>

// Sets default values for this component's properties
USteamComponent::USteamComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void USteamComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USteamComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

FString USteamComponent::GetSteamID()
{
	if (SteamAPI_Init())
	{
		uint64 sidInt = SteamUser()->GetSteamID().ConvertToUint64();

		FString SID = FString(std::to_string(sidInt).c_str());

		return SID;
	}
	
	return "";
}

FString USteamComponent::GetPlayerName()
{
	AActor* Owner = GetOwner();
	APlayerState* State = Cast<APawn>(Owner)->GetPlayerState();

	return State->GetPlayerName();
}

int USteamComponent::GetPlayerPing()
{
	AActor* Owner = GetOwner();
	APlayerState* State = Cast<APawn>(Owner)->GetPlayerState();

	return State->Ping;
}