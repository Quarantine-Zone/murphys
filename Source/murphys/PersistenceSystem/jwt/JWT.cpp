#include "JWT.h"

#include "Misc/Base64.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/pem.h>
#include <openssl/ec.h>
#include <openssl/ossl_typ.h>
#include <iomanip>
#include <sstream>
#include <string>

#include "Containers/UnrealString.h"

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Json.h"

// Constructor which requires a key to be used in hashing
JWT::JWT(FString KeyWhenHashing)
{
	HashKey = KeyWhenHashing;
}

JWT::~JWT()
{

}

// Encodes a string to base64
FString JWT::Encode64(FString Data)
{
	return FBase64::Encode(Data);
}

// Decodes a string from base64
FString JWT::Decode64(FString Data)
{
	FString Decoded;
	FBase64::Decode(Data, Decoded);

	return Decoded;
}

// Encrypts a string (payload) 
void JWT::Encrypt()
{

}

// Decrypts a string (payload)
void JWT::Decrypt()
{

}

// Hash a string using HMAC-SHA256 based hashing
FString JWT::Hash(FString TargetString)
{
	// Convert the key and target string to standard strings
	std::string targetAsStr(TCHAR_TO_UTF8(*TargetString));
	std::string keyAsStr(TCHAR_TO_UTF8(*HashKey));

	// Cast the target string to an unsigned char*
	const unsigned char* targetAsCStr = reinterpret_cast<const unsigned char*>(targetAsStr.c_str());

	// Create a new buffer for the output hash
	unsigned char outputHash[EVP_MAX_MD_SIZE];
	unsigned int HmacLen;

	// Create a new HMAC instance
	HMAC_CTX *Hmac = HMAC_CTX_new();
	HMAC_Init_ex(Hmac, keyAsStr.c_str(), keyAsStr.length(), EVP_sha256(), NULL);

	// Set the HMAC instance with the target
	HMAC_Update(Hmac, targetAsCStr, targetAsStr.size());

	// Execute the hash
	HMAC_Final(Hmac, outputHash, &HmacLen);

	// Create a string representation from the char buffer
	std::stringstream ss;
	for (unsigned int i = 0; i < HmacLen; i++)
	{
		ss << std::hex << std::setw(2) << std::setfill('0') << (int)outputHash[i];
	}

	// Free the HMAC instance
	HMAC_CTX_free(Hmac);

	// Return an FString representation
	return FString(ss.str().c_str());
}

// Returns the FString representation of an FJsonObject
void JWT::AsJsonString(FJsonObject* Target, FString* Output)
{
	// Create a new reference target and writer
	TSharedPtr<FJsonObject> TargetRef = MakeShareable(&*Target);
	TSharedRef<TJsonWriter<>> Writer = TJsonWriterFactory<>::Create(Output);

	// Serialize the object to the string!
	FJsonSerializer::Serialize(TargetRef.ToSharedRef(), Writer);
}

// Generates a JWT to be used in POST requests to the persistence API
FString JWT::GenerateToken(FJsonObject* Payload)
{
	// Construct the header
	FJsonObject Header;
	Header.SetStringField("typ", "JWT");
	Header.SetStringField("alg", "HS256");

	// Convert the header and payload to FStrings
	FString HeaderAsString;
	FString PayloadAsString;

	AsJsonString(&Header, &HeaderAsString);
	AsJsonString(Payload, &PayloadAsString);

	// Encode the header strings in base64
	FString HeaderEnc = Encode64(HeaderAsString);
	FString PayloadEnc = Encode64(PayloadAsString);

	// Create the token signature
	FString Sign = HeaderEnc.Append(".").Append(PayloadEnc);
	FString HashedSign = Hash(Sign);

	// Return the full token
	return HeaderEnc.Append(".").Append(PayloadEnc).Append(".").Append(Sign);
}

// Returns whether a token is valid based on it's provided signature
bool JWT::IsTokenValid(FString Token)
{
	// Split the token
	TArray<FString> TokenParts;
	Token.ParseIntoArray(TokenParts, TEXT("."));

	// Invalid representation
	if (TokenParts.Num() != 3)
	{
		return false;
	}

	// Assign all the parts
	FString Header = TokenParts[0];
	FString Payload = TokenParts[1];
	FString ActualSign = Decode64(TokenParts[2]);

	// Create a new signature from the header and payload
	FString CheckSign = Hash(Header.Append(".").Append(Payload));

	// Return whether the new signature is equal to the provided signature
	return CheckSign == ActualSign;
}

// Gets the FJsonObject passed in a JWT
// Does not perform validation - make sure to do this first!
void JWT::GetPayload(FString Token, FJsonObject* OutObject)
{
	// Split the token
	TArray<FString> TokenParts;
	Token.ParseIntoArray(TokenParts, TEXT("."));

	// Invalid representation
	if (TokenParts.Num() != 3)
	{
		return;
	}

	// Decode the payload
	FString Payload = Decode64(TokenParts[1]);

	AsJsonObject(Payload, OutObject);
}

void JWT::AsJsonObject(FString Target, FJsonObject* Output)
{
	// Convert the payload to an FJsonObject
	TSharedPtr<FJsonObject> PayloadAsObject = MakeShareable(Output);
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Target);
	FJsonSerializer::Deserialize(Reader, PayloadAsObject);
}