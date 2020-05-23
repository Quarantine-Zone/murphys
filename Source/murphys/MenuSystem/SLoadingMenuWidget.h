

#pragma once

#include "SlateBasics.h"
#include "SlateExtras.h"

/**
 * 
 */
class SLoadingMenuWidget : public SCompoundWidget
{
public:

	SLATE_BEGIN_ARGS(SLoadingMenuWidget) {}
	
	//SLATE_ARGUMENT(TWeakObjectPtr<class AMenuHUD>, OwningHUD)

	SLATE_END_ARGS()

	/** widget constructor**/
	void Construct(const FArguments &InArgs);

	/** format for variable passed in as arg **/
	//TWeakObjectPtr<class AMenuHUD> OwningHUD;

	virtual bool SupportsKeyboardFocus() const override { return true; };

	UPROPERTY()
	TSharedPtr<FSlateDynamicImageBrush> LoadingScreenBrush;
};

struct MurphysLoadingScreenBrush : public FSlateDynamicImageBrush, public FGCObject
{
	MurphysLoadingScreenBrush(const FName InTextureName, const FVector2D& InImageSize)
		: FSlateDynamicImageBrush(InTextureName, InImageSize)
	{
		SetResourceObject(LoadObject<UObject>(NULL, *InTextureName.ToString()));
	}

	virtual void AddReferencedObjects(FReferenceCollector& Collector)
	{
		if (UObject* CachedResourceObject = GetResourceObject())
		{
			Collector.AddReferencedObject(CachedResourceObject);
		}
	}
};