#include "Core/GlyphWeaverUtils.h"
#include "Core/GlyphWeaverSettings.h"
#include "Puzzle/GlyphPuzzle.h"
#include "Rules/GlyphPuzzleRule.h"
#include "GlyphWeaver.h"

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

void UGlyphWeaverUtils::PrintMultipleLines(const FString& InString)
{
	TArray<FString> ResultArray;
	
	InString.ParseIntoArrayLines(ResultArray);
	
	for (const FString& Line : ResultArray)
	{
		UE_LOG(LogGlyphWeaver, Verbose, TEXT("%s"), *Line);
	}
}

void UGlyphWeaverUtils::PrintGlyph(FGlyph& InGlyph)
{
	if (!IsDebugEnabled())
	{
		return;
	}
	
	PrintMultipleLines(GetGlyphString(InGlyph, 0));
}

void UGlyphWeaverUtils::PrintSequence(FGlyphSequence& InGlyphSequence)
{
	if (!IsDebugEnabled())
	{
		return;
	}
	
	PrintMultipleLines(GetSequenceString(InGlyphSequence, 0));
}

void UGlyphWeaverUtils::PrintRule(UGlyphPuzzleRule* InRule)
{
	if (!IsDebugEnabled())
	{
		return;
	}
	
	PrintMultipleLines(GetRuleString(InRule, 0));
}

void UGlyphWeaverUtils::PrintPuzzle(FGlyphPuzzle& InPuzzle)
{
	if (!IsDebugEnabled())
	{
		return;
	}
	
	PrintMultipleLines(GetPuzzleString(InPuzzle, 0));
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