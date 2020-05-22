

#pragma once

#include "jwt/JWT.h"
#include "CoreMinimal.h"
#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"

#include "Interfaces/IHttpRequest.h"
#include "Interfaces/IHttpResponse.h"

#include <vector>

/**
 * 
 */
class MURPHYS_API PersistenceManager
{
	// The JWT handler for generating tokens
	JWT* TokenHandler;

	// A map containing maps of info regarding things such as achievements, user data, etc.
	// FJsonObject allows for variable data types within it
	TMap<FString, FJsonObject*> DataCache;

	FString ErrorMessage;

	void LoadUserDataByIDCompleteInternal(TSharedPtr<FJsonObject>* Payload);
	void UpdateUserDataByIDCompleteInternal(TSharedPtr<FJsonObject>* Payload);

	void HTTPRequest(FString Endpoint, FString RequestType, FJsonObject* Payload);
	void HTTPRequestCompleteInternal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful);

public:
	PersistenceManager();
	~PersistenceManager();

	void LoadUserDataByID(FString UserID);
	void UpdateUserDataByID(FString UserID);

	DECLARE_DELEGATE_TwoParams(FPersistenceDelegate, FString, FJsonObject);

	FPersistenceDelegate OnLoadUserDataByIDCompleteDelegate;
	FPersistenceDelegate OnUpdateUserByIDCompleteDelegate;

	FJsonObject GetUserDataByID(FString UserID);
};
