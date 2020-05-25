
#pragma once

#include "jwt/JWT.h"
#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include <vector>
#include "PersistenceManager.generated.h"

DECLARE_DELEGATE_TwoParams(FPersistenceDelegate, FString, FJsonObject);

/**
 * 
 */

UCLASS(Blueprintable)
class MURPHYS_API UPersistenceManager : public UObject
{
	GENERATED_BODY()

	// The JWT handler for generating tokens
	JWT TokenHandler;

	// A map containing maps of info regarding things such as achievements, user data, etc.
	// FJsonObject allows for variable data types within it
	TMap<FString, FJsonObject> DataCache;

	FString ErrorMessage;

	void LoadUserDataByIDCompleteInternal(TSharedPtr<FJsonObject>* Payload, bool Success);
	void UpdateUserDataByIDCompleteInternal(TSharedPtr<FJsonObject>* Payload, bool Success);

	void HTTPRequest(FString Endpoint, FString RequestType, FJsonObject* Payload);
	void HTTPRequestCompleteInternal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	UFUNCTION(BlueprintCallable)
	void Init();

	UFUNCTION(BlueprintCallable)
	void LoadUserDataByID(FString UserID);

	UFUNCTION(BlueprintCallable)
	void UpdateUserDataByID(FString UserID);

	FPersistenceDelegate OnLoadUserDataByIDCompleteDelegate;
	FPersistenceDelegate OnUpdateUserByIDCompleteDelegate;

	FJsonObject GetUserDataByID(FString UserID);
};
