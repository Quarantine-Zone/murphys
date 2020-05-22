#include "PersistenceManager.h"
#include "HttpModule.h"

PersistenceManager::PersistenceManager()
{
	// TODO: Load key 
	// Instantiate JWT instance
	TokenHandler = new JWT(FString(""));

	DataCache["users"] = new FJsonObject;
}

PersistenceManager::~PersistenceManager()
{
	// Free up resources
	if (TokenHandler)
	{
		delete TokenHandler;
	}

	// Delete all references to existing objects
	for (TPair<FString, FJsonObject*> Pair : DataCache)
	{
		delete Pair.Value;
		Pair.Value = nullptr;
	}

	DataCache.Empty();
}

// Initiated internally to create an HTTP request
void PersistenceManager::HTTPRequest(FString Endpoint, FString RequestType, FJsonObject* Payload)
{
	// Generate a new token with the payload
	FString Token = TokenHandler->GenerateToken(Payload);

	// Create and process the request!
	TSharedRef<IHttpRequest> Request = FHttpModule::Get().CreateRequest();
	Request->SetVerb(RequestType);
	Request->SetURL(Endpoint);
	Request->SetHeader("Authorization", "Bearer" + Token);
	Request->OnProcessRequestComplete().BindUObject(this, &PersistenceManager::HTTPRequestCompleteInternal);
	Request->ProcessRequest();
}

// Called when an HTTP request has completed
void PersistenceManager::HTTPRequestCompleteInternal(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bWasSuccessful)
{
	// #Rip
	if (!bWasSuccessful)
	{
		return;
	}

	// Get the response and generate the json object
	FString ResponseStr = Response->GetContentAsString();
	FJsonObject Content;

	TokenHandler->AsJsonObject(ResponseStr, &Content);

	// The API will always send either a state == success or failed
	if (Content.GetStringField(FString("state")) == FString("failed"))
	{
		FString Error = Content.GetStringField("result");

		return;
	}

	// Get the token and convert it to an object
	FString Token = Content.GetStringField("result");
	FJsonObject Payload;

	TokenHandler->GetPayload(Token, &Payload);

	TSharedPtr<FJsonObject> PayloadSharable = MakeShareable(&Payload);

	// Get the request scope (verb_area)
	FJsonSerializableArray Headers = Request->GetAllHeaders();
	FString* Scope = Headers.FindByKey("RequestScope");

	// Call appropriate internal handler
	if (*Scope == FString("get_user"))
	{
		LoadUserDataByIDCompleteInternal(&PayloadSharable);
	}
	else if (*Scope == FString("update_user"))
	{
		UpdateUserDataByIDCompleteInternal(&PayloadSharable);
	}
}





void PersistenceManager::LoadUserDataByID(FString UserID)
{
	FJsonObject Payload;
	Payload.SetStringField("uid", UserID);

	HTTPRequest("https://quarantine-zone-dive.herokuapp.com/user", "GET", &Payload);
}


void PersistenceManager::LoadUserDataByIDCompleteInternal(TSharedPtr<FJsonObject>* Result)
{
	// Find returns a reference to the object (in our case already a pointer)
	FJsonObject* Users = *DataCache.Find("users");
	if (!Users || !Result->Get()) return;

	// Get the uid
	FString uid = Result->Get()->GetStringField("uid"); 
	if (uid.IsEmpty()) return;

	Users->SetObjectField(uid, *Result);

	OnLoadUserDataByIDCompleteDelegate.ExecuteIfBound(uid, *Result->Get());
}

void PersistenceManager::UpdateUserDataByIDCompleteInternal(TSharedPtr<FJsonObject>* Result)
{

}


FJsonObject PersistenceManager::GetUserDataByID(FString UserID)
{

}