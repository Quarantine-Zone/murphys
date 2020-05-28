#include "PersistenceManager.h"
#include <fstream>
#include <string>
#include <sstream>
#include "HTTP/Public/HttpModule.h"

const static FString API_BASE_URL = "http://localhost:8080/"; // api entry point
const static FString DEFAULT_UID = "test"; // for testing

void UPersistenceManager::Init()
{
	TokenHandler.SetKey(FString("jVmiggHVRJUgXGfhGVuU4u1HWGkOQo1TiucBOkU2VH761R9OdFDkvkDW7UOqzj5"));

	DataCache.Add("users", FJsonObject());

	CallbackStor.Add("get_user", &UPersistenceManager::LoadUserDataByIDCompleteInternal);
	CallbackStor.Add("update_user", &UPersistenceManager::UpdateUserDataByIDCompleteInternal);
}

// Initiated internally to create an HTTP request
void UPersistenceManager::HTTPRequest(FString Endpoint, FString RequestType, TSharedPtr<FJsonObject> Payload)
{
	// Generate a new token with the payload
	FString Token = TokenHandler.GenerateToken(Payload);

	bool Valid = TokenHandler.IsTokenValid(Token);

	// Create and process the request!
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(RequestType);
	Request->SetURL(Endpoint);
	Request->SetHeader("Authorization", "Bearer " + Token);
	Request->OnProcessRequestComplete().BindUObject(this, &UPersistenceManager::HTTPRequestCompleteInternal);
	Request->ProcessRequest();
}

// Called when an HTTP request has completed
void UPersistenceManager::HTTPRequestCompleteInternal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// #Rip
	if (!bWasSuccessful)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to perform HTTP request"));

		return;
	}

	FString Scope = Response->GetHeader("RequestScope");
	FString Bearer = Response->GetHeader("Authorization");

	// Malformated authorization header
	if (Bearer.IsEmpty())
	{
		RequestFailed(Scope);

		return;
	}

	FString Key;
	FString Token;
	Bearer.Split(TEXT(" "), &Key, &Token);

	// Malformated authorization header
	if (Token.IsEmpty())
	{
		RequestFailed(Scope);

		return;
	}

	// Validate the token
	if (!TokenHandler.IsTokenValid(Token))
	{
		RequestFailed(Scope);

		return;
	}

	// Get the payload from the token
	TSharedPtr<FJsonObject> Payload = MakeShareable(new FJsonObject());
	TokenHandler.GetPayload(Token, &Payload);

	FString state = Payload->GetStringField("state");

	// The API will always send either a state == success or failed
	if (Payload->GetStringField("state") == FString("failed"))
	{
		RequestFailed(Scope);

		return;
	}

	// Get the token and convert it to an object
	TSharedPtr<FJsonObject> Data = Payload->GetObjectField("result");
	

	// Call appropriate internal handler
	RequestSuccessful(Scope, Data);
}

// Middleware for executing callbacks successfully 
void UPersistenceManager::RequestSuccessful(FString Scope, TSharedPtr<FJsonObject> Payload)
{
	if (!CallbackStor.Contains(Scope))
	{
		UE_LOG(LogTemp, Warning, TEXT("Successful request, but no implemented handler!"));

		return;
	}

	// Call the handler with success params
	APIHandler* Fun = CallbackStor.Find(Scope);
	(this->**Fun)(Payload, true);
}

// Middleware for executing callbacks which failed 
void UPersistenceManager::RequestFailed(FString Scope)
{
	if (!CallbackStor.Contains(Scope))
	{
		return;
	}

	// Call the handler with failure params
	APIHandler* Fun = CallbackStor.Find(Scope);
	(this->* * Fun)(NULL, false);
}

// Loads data for a specific user
void UPersistenceManager::LoadUserDataByID(FString UserID)
{
	if (UserID.IsEmpty())
	{
		UserID = DEFAULT_UID;
	}

	// Create a payload and send the request
	TSharedPtr<FJsonObject> Payload = MakeShareable(new FJsonObject());
	Payload->SetStringField("uid", UserID);

	HTTPRequest(API_BASE_URL + "user", "GET", Payload);
}

// Internal handler for when user data has been loaded
void UPersistenceManager::LoadUserDataByIDCompleteInternal(TSharedPtr<FJsonObject> Result, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to load user data"));

		return;
	}

	// Find returns a reference to the object (in our case already a pointer)
	FJsonObject* Users = DataCache.Find("users");
	if (!Users || !Result) return;

	// Get the uid
	FString uid = Result->GetStringField("uid"); 
	if (uid.IsEmpty()) return;

	// Cache the returned data
	Users->SetObjectField(uid, Result);
	UE_LOG(LogTemp, Warning, TEXT("Loaded user data for %s"), *uid);

	// Call any bound delegates
	OnLoadUserDataByIDCompleteDelegate.ExecuteIfBound(uid, Result);
}

// Update user data
void UPersistenceManager::UpdateUserDataByID(FString UserID, FPersistenceUserData ToUpdate)
{
	if (UserID.IsEmpty())
	{
		UserID = DEFAULT_UID;
	}

	FJsonObject* Users = DataCache.Find("users");
	if (!Users) return;

	// Make sure the user's data is actually loaded
	TSharedPtr<FJsonObject> UserData = Users->GetObjectField(UserID);
	if (!UserData.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("Attempted to update user data but none is loaded or invalid user id!"));
		return;
	}

	if (ToUpdate.NumberValues.Num() == 0 && ToUpdate.StringValues.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("Nothing to update"));
		return;
	}

	// This sucks, I know :(, but FJSonObjects don't exist on blueprints, so we have to use our own struct
	// We'll just use them for communication and  data transmission locally
	TSharedPtr<FJsonObject> PayloadUpdate = MakeShareable(new FJsonObject());
	for (auto It = ToUpdate.NumberValues.CreateConstIterator(); It; ++It)
	{
		if (It.Key().IsEmpty())
		{
			continue;
		}

		PayloadUpdate->SetNumberField(It.Key(), It.Value());
	}

	for (auto It = ToUpdate.StringValues.CreateConstIterator(); It; ++It)
	{
		if (It.Key().IsEmpty())
		{
			continue;
		}

		PayloadUpdate->SetStringField(It.Key(), It.Value());
	}

	// Create the payload
	TSharedPtr<FJsonObject> Payload = MakeShareable(new FJsonObject());
	Payload->SetStringField("uid", UserID);
	Payload->SetObjectField("toUpdate", PayloadUpdate);

	// Iterate over the payload values and update the user data cache
	for (auto It = PayloadUpdate->Values.CreateConstIterator(); It; ++It)
	{
		UserData->SetField(It.Key(), It.Value());
	}

	// Make the request!
	HTTPRequest(API_BASE_URL + "user", "POST", Payload);
}

// Internal handler for when data has been updated
void UPersistenceManager::UpdateUserDataByIDCompleteInternal(TSharedPtr<FJsonObject> Result, bool Success)
{
	// Successful request, no need to do anything
	FString uid = Result->GetStringField("uid");

	if (Success && !uid.IsEmpty())
	{
		UE_LOG(LogTemp, Warning, TEXT("Updated user data for %s"), *Result->GetStringField("uid"));

		OnUpdateUserByIDCompleteDelegate.ExecuteIfBound(uid, Result);

		return;
	}

	// TODO: Handle inconsistencies (try again, etc.)
	UE_LOG(LogTemp, Warning, TEXT("Failed to update user data - local data out of sync!"));
}

// Returns a copy of the data cached for this user
// Note: Will return an empty object is nothing if there's a problem
FPersistenceUserData UPersistenceManager::GetUserDataByID(FString UserID)
{
	if (UserID.IsEmpty())
	{
		UserID = DEFAULT_UID;
	}

	FPersistenceUserData PersistenceData;

	// Make sure users exist
	FJsonObject* Users = DataCache.Find("users");
	if (!Users) return PersistenceData;

	// Make sure the user's data is actually loaded
	TSharedPtr<FJsonObject> UserData = Users->GetObjectField(UserID);
	if (!UserData.IsValid()) return PersistenceData;

	// Construct return structure
	for (auto It = UserData->Values.CreateConstIterator(); It; ++It)
	{
		// Make sure we are only dealing with numbers and strings for now
		EJson Type = It.Value().Get()->Type;

		// Add appropriate value by type
		if (Type == EJson::String) 
		{
			PersistenceData.StringValues.Add(It.Key(), It.Value().Get()->AsString());
		}
		else if (Type == EJson::Number)
		{
			PersistenceData.NumberValues.Add(It.Key(), It.Value().Get()->AsNumber());
		}
	}

	return PersistenceData;
}