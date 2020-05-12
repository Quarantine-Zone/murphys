
#pragma once

#include "CoreMinimal.h"
#include "MessagePart.h"
#include "Styling/SlateColor.h"
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

	void Translate(FString Format, UMessagePart* Row);
	bool IsValidFormat(FString Format);
};
