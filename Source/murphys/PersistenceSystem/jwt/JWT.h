

#pragma once

#include "CoreMinimal.h"
#include "Json.h"
#include <string>

/**
 * 
 */
class MURPHYS_API JWT
{
	FString HashKey;

	// Encodes a string to base 64
	FString Encode64(FString Data);

	// Decodes a string from base 64
	FString Decode64(FString Data);

	// Encrypts/Decrypts the payload
	// TODO: Implementation
	void Encrypt();
	void Decrypt();

	// Hashes a string
	FString Hash(FString TargetString);

	// Serializes a json object to a string
	FString AsJsonString(TSharedPtr<FJsonObject> Target);

public:
	// Sets the key to hash
	void SetKey(FString KeyToUse);
	
	// Generates a JWT
	FString GenerateToken(TSharedPtr<FJsonObject> Payload);
	
	// Gets the payload of a token
	void GetPayload(FString Token, TSharedPtr<FJsonObject>* OutObject);

	// Returns whether a token is valid
	bool IsTokenValid(FString Token);

	// Deserializes a json string to an object
	void AsJsonObject(FString Target, TSharedPtr<FJsonObject>* Output);
};
