
#pragma once

#include "CoreMinimal.h"
#include "MessagePart.h"
#include <vector>
#include <map>

typedef void (*pfunc)(UMessagePart*);

/**
 * 
 */
class MURPHYS_API MessageFormat
{
	std::map<FString, pfunc> Formats;

public:
	MessageFormat();

	void Translate(FString Format, UMessagePart* RowPart);
	bool IsValidFormat(FString Format);
};
