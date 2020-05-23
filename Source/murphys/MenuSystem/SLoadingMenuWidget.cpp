


#include "SLoadingMenuWidget.h"
#include "Math/UnrealMathUtility.h"

#define LOCTEXT_NAMESPACE "LoadingMenu"

void SLoadingMenuWidget::Construct(const FArguments &InArgs)
{
	// some fun flavor text for the loadng menu
	
	int MessageListLen = 9;
	FString MessageList [] = 
	{ 
		"Here's something interesting to read while you wait",
		"Fun Fact: Larvin Mewis was the best coach the Bengals ever had.",
		"No one expects the Spanish Inquisition",
		"Ethan is Ok sometines",
		"There are 10 types of people in the world, those who understand binary, and those who don't",
		"What does Coors Light have in common with people making love in a cannoe? ...They're fucking close to water",
		"Do it, you won't",
		"It's all fun and games until someone gets a seg fault.",
		"Thank you for reading this"
	};
	
	int i = rand() % MessageListLen;
	
	const FText LoadingPrompt = LOCTEXT("LoadingPromt", "Loading...");
	//const FText LoadingMessage = FText::FromString(MessageList[i]);
	//const FText LoadingMessage = LOCTEXT("LoadingMessage", "Here's something interesting to read while you wait");
	const FText LoadingMessage = FText::FromString(MessageList[i]);

	const FMargin ContentPadding = FMargin(20.f);

	FSlateFontInfo PromptTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	PromptTextStyle.Size = 40.0f;

	FSlateFontInfo MessageTextStyle = PromptTextStyle;
	MessageTextStyle.Size = 24.0f;

	static const FName LoadingScreenName(TEXT("/Game/MenuSystem/Images/cocktail-bar-background.cocktail-bar-background"));
	LoadingScreenBrush = MakeShareable(new MurphysLoadingScreenBrush(LoadingScreenName, FVector2D(1500, 940)));

	ChildSlot
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				[
				SNew(SImage)
				.Image(LoadingScreenBrush.Get())
				]
			]
			+ SOverlay::Slot()
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Bottom)
			[
				SNew(SOverlay)
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				[
					SNew(SImage)
					.ColorAndOpacity(FColor::FromHex("000000AA"))
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Fill)
				.VAlign(VAlign_Fill)
				.Padding(ContentPadding)
				[
					SNew(SVerticalBox)
					+ SVerticalBox::Slot()
					.Padding(FMargin(0.f, 20.f))
					[
						SNew(STextBlock)
						.Font(PromptTextStyle)
						.Text(LoadingPrompt)
					]
					+ SVerticalBox::Slot()
					[
						SNew(STextBlock)
						.Font(MessageTextStyle)
						.Text(LoadingMessage)
					]
				]
				+ SOverlay::Slot()
				.HAlign(HAlign_Right)
				.VAlign(VAlign_Center)
				.Padding(20.f)
				[
					SNew(SCircularThrobber)
					.Radius(30.f)
				]
			]
		];
}

#undef LOCTEXT_NAMESPACE
