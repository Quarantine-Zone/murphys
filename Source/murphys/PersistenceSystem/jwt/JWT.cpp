#include "JWT.h"

#include "Misc/Base64.h"
#include "Serialization/JsonWriter.h"
#include "Serialization/JsonSerializer.h"

#include "Policies/CondensedJsonPrintPolicy.h"

#include <openssl/sha.h>
#include <openssl/evp.h>
#include <openssl/hmac.h>
#include <openssl/pem.h>
#include <openssl/ec.h>
#include <openssl/ossl_typ.h>

#include <string>

#include "Containers/UnrealString.h"

#include "Dom/JsonObject.h"
#include "Dom/JsonValue.h"
#include "Json.h"

// Constructor which requires a key to be used in hashing
void JWT::SetKey(FString KeyWhenHashing)
{
	HashKey = KeyWhenHashing;
}

// Encodes a string to base64
FString JWT::Encode64(FString Data)
{
	FString Encoded = FBase64::Encode(Data);
	Encoded = Encoded.Replace(TEXT("+"), TEXT("-")).Replace(TEXT("/"), TEXT("_")).Replace(TEXT("="), TEXT(""));

	return Encoded;
}

// Decodes a string from base64
FString JWT::Decode64(FString Data)
{
	Data = Data.Replace(TEXT("-"), TEXT("+")).Replace(TEXT("_"), TEXT("/"));

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

	// Create a new buffer for the output hash
	unsigned char outputHash[64];
	unsigned int HmacLen;

	// Create a new HMAC instance
	HMAC_CTX *Hmac = HMAC_CTX_new();
	HMAC_Init_ex(Hmac, &keyAsStr[0], keyAsStr.length(), EVP_sha256(), NULL);

	// Set the HMAC instance with the target
	HMAC_Update(Hmac, (unsigned char*)&targetAsStr[0], targetAsStr.length());

	// Execute the hash
	HMAC_Final(Hmac, outputHash, &HmacLen);
	
	// Create a string representation from the char buffer	
	// This took me way too long to realize I needed to convert this to base 64
	TArray<uint8> HashAsBytes;
	for (unsigned int i = 0; i < HmacLen; i++)
	{
		// Convert the character array to a byte array
		HashAsBytes.Add((uint8)outputHash[i]);
	}

	// Convert the entire array of bytes to a base64 string
	// Note: FString doesn't like non-printable characters, so we have to encode it separately (from our normal strings) here AND make it url safe
	FString Encoded = FBase64::Encode(HashAsBytes).Replace(TEXT("+"), TEXT("-")).Replace(TEXT("/"), TEXT("_")).Replace(TEXT("="), TEXT(""));

	// Free the HMAC instance
	HMAC_CTX_free(Hmac);

	// Return an FString representation
	return Encoded;
}

// Returns the FString representation of an FJsonObject
FString JWT::AsJsonString(TSharedPtr<FJsonObject> Target)
{
	// Create a new reference target and writer
	FString Output;

	TSharedRef< TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>> > Writer = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR> >::Create(&Output);

	// Serialize the object to the string!
	FJsonSerializer::Serialize(Target.ToSharedRef(), Writer);
	
	return Output;
}

// Generates a JWT to be used in POST requests to the persistence API
FString JWT::GenerateToken(TSharedPtr<FJsonObject> Payload)
{
	// Construct the header
	TSharedPtr<FJsonObject> Header = MakeShareable(new FJsonObject);
	Header->SetStringField("typ", "JWT");
	Header->SetStringField("alg", "HS256");

	// Convert the header and payload to FStrings
	FString HeaderAsString = AsJsonString(Header);
	FString PayloadAsString = AsJsonString(Payload);

	// Encode the header strings in base64
	FString HeaderEnc = Encode64(HeaderAsString);
	FString PayloadEnc = Encode64(PayloadAsString);

	FString Sign = HeaderEnc + "." + PayloadEnc;

	// Create the token signature
	// Note: Already returned in base 64 (dealing with bytes, FString doesn't like non-printable characters)
	FString HashedSign = Hash(Sign);

	FString Token = HeaderEnc + "." + PayloadEnc + "." + HashedSign;

	// Set the output as the token value
	return Token;
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
	FString ActualSign = TokenParts[2];

	// Create a new signature from the header and payload
	FString CheckSign = Hash(Header.Append(".").Append(Payload));

	// Return whether the new signature is equal to the provided signature
	return CheckSign == ActualSign;
}

// Gets the FJsonObject passed in a JWT
// Does not perform validation - make sure to do this first!
void JWT::GetPayload(FString Token, TSharedPtr<FJsonObject>* OutObject)
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

// Deserializes a json string to a JSON object
void JWT::AsJsonObject(FString Target, TSharedPtr<FJsonObject>* Output)
{
	// Convert the payload to an FJsonObject
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Target);
	bool success = FJsonSerializer::Deserialize(Reader, *Output);
}