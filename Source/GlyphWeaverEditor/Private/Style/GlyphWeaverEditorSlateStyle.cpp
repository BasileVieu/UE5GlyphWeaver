#include "Style/GlyphWeaverEditorSlateStyle.h"
#include "Styling/CoreStyle.h"
#include "Styling/SlateStyleRegistry.h"
#include "Styling/SlateTypes.h"

TSharedPtr<FSlateStyleSet> FGlyphWeaverEditorSlateStyle::StyleInstance = nullptr;

void FGlyphWeaverEditorSlateStyle::Initialize()
{
	if (!StyleInstance.IsValid())
	{
		StyleInstance = Create();
		
		FSlateStyleRegistry::RegisterSlateStyle(*StyleInstance);
	}
}

void FGlyphWeaverEditorSlateStyle::Shutdown()
{
	if (StyleInstance.IsValid())
	{
		FSlateStyleRegistry::UnRegisterSlateStyle(*StyleInstance);
		
		StyleInstance.Reset();
	}
}

const ISlateStyle& FGlyphWeaverEditorSlateStyle::Get()
{
	return *StyleInstance;
}

FName FGlyphWeaverEditorSlateStyle::GetStyleSetName()
{
	static FName StyleSetName("GlyphWeaverEditorStyle");
 	
 	return StyleSetName;
}

TSharedRef<FSlateStyleSet> FGlyphWeaverEditorSlateStyle::Create()
{
	TSharedRef<FSlateStyleSet> Style = MakeShared<FSlateStyleSet>(GetStyleSetName());
	
	Style->Set("CurrentPuzzleDataText", FTextBlockStyle()
		.SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 10, "Bold"))
		.SetColorAndOpacity(FSlateColor(FLinearColor::Blue))
		.SetShadowOffset(FVector2D(1.0f, 1.0f)));
	
	Style->Set("PuzzleDataText", FTextBlockStyle()
		.SetFont(FSlateFontInfo(FCoreStyle::GetDefaultFont(), 10, "Bold"))
		.SetColorAndOpacity(FSlateColor(FLinearColor::Red))
		.SetShadowOffset(FVector2D(1.0f, 1.0f)));
	
	return Style;
}