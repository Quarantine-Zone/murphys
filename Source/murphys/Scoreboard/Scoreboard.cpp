


#include "Scoreboard.h"
#include "Blueprint/WidgetTree.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UScoreboard::AddColumn(FString Title)
{
	Columns.Add(Title);
}

void UScoreboard::SetUseCharacter(bool ShouldUseCharacter)
{
	UseCharacter = ShouldUseCharacter;
}

void UScoreboard::SetHeaders()
{
	// Calculate the maximum space each column can take
	float SizeWidth = ScoreboardWidth / Columns.Num();

	// Loop through each of our columns
	for (FString Column : Columns) {
		// Create a wrapper
		USizeBox* TextWrapper = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		TextWrapper->SetWidthOverride(SizeWidth);
		if (!ensure(TextWrapper != nullptr)) return;

		// The actual text column text
		UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Text->SetText(FText::FromString(Column));
		if (!ensure(Text != nullptr)) return;
		
		// Set children
		TextWrapper->AddChild(Text);

		if (!ensure(HeadersContainer != nullptr)) return;
		HeadersContainer->AddChild(TextWrapper);
	}
}

void UScoreboard::AddRow(FScoreboardEntry* Entry)
{
	// Calculate the maximum space each column can take 
	float SizeWidth = ScoreboardWidth / Columns.Num();

	// Get the row data for this entry
	TArray<FString> RowEntries = GetColumnData(Entry->UseCharacter, Entry->Character, Entry->Pawn, Columns);

	// Create a wrapper for the row
	UHorizontalBox* RowWrapper = WidgetTree->ConstructWidget<UHorizontalBox>(UHorizontalBox::StaticClass());
	if (!ensure(RowWrapper != nullptr)) return;

	for (FString RowValue : RowEntries) {
		// Create the wrapper for this text
		USizeBox* TextWrapper = WidgetTree->ConstructWidget<USizeBox>(USizeBox::StaticClass());
		TextWrapper->SetWidthOverride(SizeWidth);
		if (!ensure(TextWrapper != nullptr)) return;

		// Create the actual text value for this column
		UTextBlock* Text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass());
		Text->SetText(FText::FromString(RowValue));
		if (!ensure(Text != nullptr)) return;

		// Add children
		TextWrapper->AddChild(Text);
		RowWrapper->AddChild(TextWrapper);
	}

	// Add it to the scoreboard!
	if (!ensure(RowContainer != nullptr)) return;
	RowContainer->AddChild(RowWrapper);
}

void UScoreboard::Init()
{
	// Create our headers
	SetHeaders();

	// Start creating rows based on if we want to pass a character or pawn reference
	if (UseCharacter) 
	{
		TArray<AActor*> FoundCharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMurphysBaseCharacter::StaticClass(), FoundCharacters);

		// Create a row for each character
		for (auto Character : FoundCharacters) {
			FScoreboardEntry Entry;
			Entry.UseCharacter = true;
			Entry.Character = Cast<AMurphysBaseCharacter>(Character);

			AddRow(&Entry);
		}
	}
	else
	{
		TArray<AActor*> FoundCharacters;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMurphysBasePawn::StaticClass(), FoundCharacters);

		// Create a row for each pawn
		for (auto Character : FoundCharacters) {
			FScoreboardEntry Entry;
			Entry.UseCharacter = false;
			Entry.Pawn = Cast<AMurphysBasePawn>(Character);

			AddRow(&Entry);
		}
	}
}