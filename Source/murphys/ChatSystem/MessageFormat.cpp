

#include "MessageFormat.h"
#include "Math/Color.h"

#include "Components/TextBlock.h"

// Register translations
MessageFormat::MessageFormat()
{
	Formats["default"] = [](UMessagePart* Row) {

	};

	Formats[":green"] = [](UMessagePart* Row) {
		FLinearColor Color(0, 128, 0, 255);
		Row->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":red"] = [](UMessagePart* Row) {
		FLinearColor Color(255, 0, 0, 255);
		Row->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":yellow"] = [](UMessagePart* Row) {
		FLinearColor Color(255, 255, 0, 255);
		Row->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":blue"] = [](UMessagePart* Row) {
		FLinearColor Color(0, 0, 255, 255);
		Row->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":pink"] = [](UMessagePart* Row) {
		FLinearColor Color(255, 20, 147, 255);
		Row->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":orange"] = [](UMessagePart* Row) {
		FLinearColor Color(255, 165, 0, 255);
		Row->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":rainbow"] = [](UMessagePart* Row) {
		Row->Text->
	};
}

// Actually perform the translation on the row
void MessageFormat::Translate(FString Format, UMessagePart* Row)
{
	if (!IsValidFormat(Format)) return;

	// Call the translator
	(*(Formats[Format]))(Row);
}

bool MessageFormat::IsValidFormat(FString Format)
{
	return Formats.find(Format) != Formats.end();
}
