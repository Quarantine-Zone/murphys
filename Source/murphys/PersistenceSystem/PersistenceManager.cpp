#include "PersistenceManager.h"
#include <fstream>
#include <string>
#include <sstream>
#include "HTTP/Public/HttpModule.h"

void UPersistenceManager::Init()
{
	std::ifstream infile("persistence_key.txt");
	if (infile.good())
	{
		std::string line;
		std::getline(infile, line);

		TokenHandler.SetKey(FString(line.c_str()));

		FJsonObject Users;
		DataCache["users"] = Users;
	} 
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't load persistence API secret!"));
	}
}

// Initiated internally to create an HTTP request
void UPersistenceManager::HTTPRequest(FString Endpoint, FString RequestType, FJsonObject* Payload)
{
	// Generate a new token with the payload
	FString Token = TokenHandler.GenerateToken(Payload);

	// Create and process the request!
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(RequestType);
	Request->SetURL(Endpoint);
	Request->SetHeader("Authorization", "Bearer" + Token);
	Request->OnProcessRequestComplete().BindUObject(this, &UPersistenceManager::HTTPRequestCompleteInternal);
	Request->ProcessRequest();
}

// Called when an HTTP request has completed
void UPersistenceManager::HTTPRequestCompleteInternal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// #Rip
	if (!bWasSuccessful)
	{
		return;
	}

	// Get the response and generate the json object
	FString ResponseStr = Response->GetContentAsString();
	FJsonObject Content;

	TokenHandler.AsJsonObject(ResponseStr, &Content);

	// The API will always send either a state == success or failed
	if (Content.GetStringField(FString("state")) == FString("failed"))
	{
		FString Error = Content.GetStringField("result");

		return;
	}

	// Get the token and convert it to an object
	FString Token = Content.GetStringField("result");
	FJsonObject Payload;

	TokenHandler.GetPayload(Token, &Payload);

	TSharedPtr<FJsonObject> PayloadSharable = MakeShareable(&Payload);

	// Get the request scope (verb_area)
	FJsonSerializableArray Headers = Request->GetAllHeaders();
	FString* Scope = Headers.FindByKey("RequestScope");

	// Call appropriate internal handler
	if (*Scope == FString("get_user"))
	{
		LoadUserDataByIDCompleteInternal(&PayloadSharable, true);
	}
	else if (*Scope == FString("update_user"))
	{
		UpdateUserDataByIDCompleteInternal(&PayloadSharable, true);
	}
}

void UPersistenceManager::LoadUserDataByID(FString UserID)
{
	FJsonObject Payload;
	Payload.SetStringField("uid", UserID);

	HTTPRequest("https://quarantine-zone-dive.herokuapp.com/user", "GET", &Payload);
}

void UPersistenceManager::LoadUserDataByIDCompleteInternal(TSharedPtr<FJsonObject>* Result, bool Success)
{
	// Find returns a reference to the object (in our case already a pointer)
	FJsonObject* Users = DataCache.Find("users");
	if (!Users || !Result->Get()) return;

	// Get the uid
	FString uid = Result->Get()->GetStringField("uid"); 
	if (uid.IsEmpty()) return;

	Users->SetObjectField(uid, *Result);

	OnLoadUserDataByIDCompleteDelegate.ExecuteIfBound(uid, *Result->Get());
}

void UPersistenceManager::UpdateUserDataByIDCompleteInternal(TSharedPtr<FJsonObject>* Result, bool Success)
{

}

void UPersistenceManager::UpdateUserDataByID(FString UserID)
{

}

FJsonObject UPersistenceManager::GetUserDataByID(FString UserID)
{
	FJsonObject obj;

	return obj;
}