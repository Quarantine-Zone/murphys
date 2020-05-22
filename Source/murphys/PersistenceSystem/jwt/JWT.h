

#pragma once

#include "CoreMinimal.h"
#include "Json.h"

/**
 * 
 */
class MURPHYS_API JWT
{
	FString HashKey;

	FString Encode64(FString Data);
	FString Decode64(FString Data);

	void Encrypt();
	void Decrypt();

	FString Hash(FString TargetString);

	void AsJsonString(FJsonObject* Target, FString* Output);

public:
	JWT(FString KeyWhenHashing);
	~JWT();
	
	FString GenerateToken(FJsonObject* Payload);
	void GetPayload(FString Token, FJsonObject* OutObject);

	bool IsTokenValid(FString Token);

	void AsJsonObject(FString Target, FJsonObject* Output);
};
