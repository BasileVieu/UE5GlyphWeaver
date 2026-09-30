#include "Core/GlyphWeaverUtils.h"
#include "Core/GlyphWeaverSettings.h"
#include "Puzzle/GlyphPuzzle.h"
#include "Rules/GlyphPuzzleRule.h"

DEFINE_LOG_CATEGORY(LogGlyphWeaver);

bool UGlyphWeaverUtils::IsDebugEnabled()
{
	const UGlyphWeaverSettings* Settings = GetDefault<UGlyphWeaverSettings>();

	return Settings
		&& Settings->DebugEnabled;
}

FString UGlyphWeaverUtils::GetIndent(int InDepth)
{
	return FString::ChrN(InDepth, TEXT('\t'));
}

FString UGlyphWeaverUtils::GetGlyphString(FGlyph& InGlyph, int InIndent)
{
	const FString Tabs = GetIndent(InIndent);
	
	return FString::Printf(TEXT("%sGlyph: Name=%s Value=%d\n"),
		*Tabs,
		*InGlyph.Name.ToString(),
		InGlyph.Value);
}

FString UGlyphWeaverUtils::GetSequenceString(FGlyphSequence& InGlyphSequence, int InIndent)
{
	const FString Tabs = GetIndent(InIndent);
	
	if (InGlyphSequence.Size() == 0)
	{
		return FString::Printf(TEXT(
			"%sSequence: Name=%s. This sequence is empty."),
			*Tabs,
			*InGlyphSequence.Name.ToString());
	}
	
	FString Result;
	
	Result += FString::Printf(TEXT("%sSequence: Name=%s Size=%d\n"),
		*Tabs,
		*InGlyphSequence.Name.ToString(),
		InGlyphSequence.Size());
	
	for (int i = 0; i < InGlyphSequence.Size(); i++)
	{
		Result += GetGlyphString(InGlyphSequence.Get(i), InIndent + 1);
		
		if (i < InGlyphSequence.Size() - 1)
		{
			Result += TEXT("\n");
		}
	}
	
	return Result;
}

FString UGlyphWeaverUtils::GetRuleString(UGlyphPuzzleRule* InRule, int InIndent)
{
	const FString Tabs = GetIndent(InIndent);
	
	FString Result;
	
	Result += FString::Printf(TEXT("%sRule: Class=%s\n"),
		*Tabs,
		*InRule->GetClass()->GetName());
	
	Result += FString::Printf(TEXT("%s\tApply to target? %s\n"),
		*Tabs,
		InRule->ApplyToTarget ? TEXT("Yes") : TEXT("No"));
	
	Result += FString::Printf(TEXT("%s\tApply to guess? %s\n"),
		*Tabs,
		InRule->ApplyToGuess ? TEXT("Yes") : TEXT("No"));
	
	return Result;
}

FString UGlyphWeaverUtils::GetPuzzleString(FGlyphPuzzle& InPuzzle, int InIndent)
{
	const FString Tabs = GetIndent(InIndent);
	
	FString Result;
	
	Result += FString::Printf(TEXT("%sPuzzle: Name=%s\n"),
		*Tabs,
		*InPuzzle.Name.ToString());
	
	Result += FString::Printf(TEXT("%s\tState=%s\n"),
		*Tabs,
		InPuzzle.Solved ? TEXT("Solved") : TEXT("Unsolved"));
	
	Result += GetSequenceString(InPuzzle.Sequence, InIndent + 1);
	
	for (int i = 0; i < InPuzzle.Rules.Num(); i++)
	{
		Result += GetRuleString(InPuzzle.Rules[i], InIndent + 1);
		
		if (i < InPuzzle.Rules.Num() - 1)
		{
			Result += TEXT("\n");
		}
	}
	
	return Result;
}