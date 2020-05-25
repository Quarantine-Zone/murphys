

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SteamComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MURPHYS_API USteamComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USteamComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	uint64 GetSteamID();

	FString GetPlayerName();
	
	int GetPlayerPing();
};
