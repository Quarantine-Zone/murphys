
#pragma once

#include "jwt/JWT.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"

#include "Json.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include <vector>
#include "PersistenceManager.generated.h"

DECLARE_DELEGATE_TwoParams(FPersistenceDelegate, FString, TSharedPtr<FJsonObject>);

typedef void (UPersistenceManager::*APIHandler)(TSharedPtr<FJsonObject> Payload, bool Success);

// FJsonObjects don't exist on blueprints
USTRUCT(BlueprintType)
struct FPersistenceUserData
{
	GENERATED_BODY()

public:
	// String values for a user
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, FString> StringValues;
	
	// Number values for a user
	UPROPERTY(BlueprintReadWrite)
	TMap<FString, float> NumberValues;
};

/**
 * 
 */

UCLASS(Blueprintable)
class MURPHYS_API UPersistenceManager : public UObject
{
	GENERATED_BODY()

	// The JWT handler for generating tokens
	JWT TokenHandler;

	// Map caching scope (create_user, etc.) to a function callback
	TMap<FString, APIHandler> CallbackStor;

	// A map containing maps of info regarding things such as achievements, user data, etc.
	// FJsonObject allows for variable data types within it
	TMap<FString, FJsonObject> DataCache;

	FString ErrorMessage;

	// Middleware to map successful/failed requests to appropriate handlers
	void RequestSuccessful(FString Scope, TSharedPtr<FJsonObject> Payload);
	void RequestFailed(FString Scope);

	// Internal handlers
	void LoadUserDataByIDCompleteInternal(TSharedPtr<FJsonObject> Payload, bool Success);
	void UpdateUserDataByIDCompleteInternal(TSharedPtr<FJsonObject> Payload, bool Success);

	// Internal HTTP handlers
	void HTTPRequest(FString Endpoint, FString RequestType, TSharedPtr<FJsonObject> Payload);
	void HTTPRequestCompleteInternal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void LoadUserDataByID(FString UserID);

	UFUNCTION(BlueprintCallable)
	void UpdateUserDataByID(FString UserID, FPersistenceUserData ToUpdate);

	FPersistenceDelegate OnLoadUserDataByIDCompleteDelegate;
	FPersistenceDelegate OnUpdateUserByIDCompleteDelegate;

	UFUNCTION(BlueprintCallable)
	FPersistenceUserData GetUserDataByID(FString UserID);
};
