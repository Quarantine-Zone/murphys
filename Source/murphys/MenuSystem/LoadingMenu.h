

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingMenu.generated.h"

/**
 * 
 */
UCLASS()
class MURPHYS_API ULoadingMenu : public UUserWidget
{
	GENERATED_BODY()
	


public:
	
	void Setup();

	void Teardown();

};
