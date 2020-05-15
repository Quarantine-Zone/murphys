

#include "MessageFormat.h"
#include "Math/Color.h"
#include "Styling/SlateColor.h"
#include "Components/TextBlock.h"

// Register translations
MessageFormat::MessageFormat()
{
	Formats["default"] = [](UMessagePart* RowPart) {

	};

	Formats[":green"] = [](UMessagePart* RowPart) {
		FLinearColor Color(0, .5, 0, 1);
		RowPart->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":red"] = [](UMessagePart* RowPart) {
		FLinearColor Color(1, 0, 0, 1);
		RowPart->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":yellow"] = [](UMessagePart* RowPart) {
		FLinearColor Color(1, 1, 0, 1);
		RowPart->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":blue"] = [](UMessagePart* RowPart) {
		FLinearColor Color(0, 0, 1, 1);
		RowPart->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":pink"] = [](UMessagePart* RowPart) {
		FLinearColor Color(1, 0, 1, 1);
		RowPart->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":orange"] = [](UMessagePart* RowPart) {
		FLinearColor Color(1, .65, 0, 1);
		RowPart->Text->SetColorAndOpacity(FSlateColor::FSlateColor(Color));
	};

	Formats[":rainbow"] = [](UMessagePart* RowPart) {
		RowPart->UseRainbow = true;
	};
}

// Actually perform the translation on the RowPart
void MessageFormat::Translate(FString Format, UMessagePart* RowPart)
{
	if (!IsValidFormat(Format)) return;

	// Call the translator
	(*(Formats[Format]))(RowPart);
}

bool MessageFormat::IsValidFormat(FString Format)
{
	return Formats.find(Format) != Formats.end();
}
