

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UObject/Object.h"
#include "Components/ScrollBox.h"
#include "Components/HorizontalBox.h"
#include "../Character/MurphysBaseCharacter.h"
#include "../Character/MurphysBasePawn.h"
#include "Scoreboard.generated.h"

// Used internally 
USTRUCT(BlueprintType)
struct FScoreboardEntry {
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool UseCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMurphysBaseCharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AMurphysBasePawn* Pawn;
};

/**
 * 
 */
UCLASS()
class MURPHYS_API UScoreboard : public UUserWidget
{
	GENERATED_BODY()
	
	// Record of existing headers
	TArray<FString> Columns;

	// Whether to use the character(t) or pawn (f) (since neither class inherits from the other)
	bool UseCharacter = true;

	// Creates the headers
	void SetHeaders();

	// Adds a row
	void AddRow(FScoreboardEntry* Entry);
public:
	// Sets whether we use a character or not
	UFUNCTION(BlueprintCallable)
	void SetUseCharacter(bool ShouldUseCharacter);

	// Should be overriden in blueprint, responsible for getting the row values based on column headers
	UFUNCTION(BlueprintImplementableEvent)
	TArray<FString> GetColumnData(bool UseAsCharacter, AMurphysBaseCharacter* AsCharacter, AMurphysBasePawn* AsPawn, const TArray<FString> &ColumnHeaders);

	// Adds a new column
	UFUNCTION(BlueprintCallable)
	void AddColumn(FString Title);

	// Constructs the scoreboard
	UFUNCTION(BlueprintCallable)
	void Init();

	// Property bindings
	UPROPERTY(meta = (BindWidget))
	UScrollBox* RowContainer;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* HeadersContainer;

	UPROPERTY(BlueprintReadWrite)
	float ScoreboardWidth = 900;
};
