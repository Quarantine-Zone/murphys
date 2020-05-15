

#include "MessageRow.h"

UMessageRow::UMessageRow(const FObjectInitializer& ObjectInitializer) : UUserWidget(ObjectInitializer)
{

}

void UMessageRow::SetSenderName(FString Nick)
{
	MessageSender->SetText(FText::FromString(Nick + ":"));
}