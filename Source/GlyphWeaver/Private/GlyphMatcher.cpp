#include "GlyphMatcher.h"
#include "GlyphPuzzle/GlyphPuzzle.h"
#include "GlyphPuzzle/GlyphPuzzleRule.h"

bool UGlyphMatcher::Matches(FGlyphSequence& InTargetSequence, FGlyphSequence& InGuessSequence, int InModulo, TArray<UGlyphPuzzleRule*> InRules)
{
	if (InTargetSequence.Size() > InGuessSequence.Size())
	{
		return false;
	}
	
	TArray<int> TargetBaseValues;
	
	for (FGlyph& Glyph : InTargetSequence.Glyphs)
	{
		TargetBaseValues.Add(Glyph.Value);
	}
	
	TArray<int> GuessBaseValues;
	
	for (FGlyph& Glyph : InGuessSequence.Glyphs)
	{
		GuessBaseValues.Add(Glyph.Value);
	}
	
	FGlyphVariants Target(TargetBaseValues);
	FGlyphVariants Guess(GuessBaseValues);
	
	for (UGlyphPuzzleRule* Rule : InRules)
	{
		if (Rule->ApplyToTarget)
		{
			Target = Rule->Apply(Target, InModulo, InTargetSequence.Size());
		}
		
		if (Rule->ApplyToGuess)
		{
			Guess = Rule->Apply(Guess, InModulo, InTargetSequence.Size());
		}
	}
	
	for (int i = 0; i < Target.Variants.Num(); i++)
	{
		for (int j = 0; j < Guess.Variants.Num(); j++)
		{
			if (Target.Variants[i] == Guess.Variants[j])
			{
				return true;
			}
		}
	}
	
	return false;
}

void UGlyphMatcher::PrintSequence(const TArray<int>& InSequence, const FString& InName)
{
	UE_LOG(LogTemp, Warning, TEXT("%s count: %d\n"), *InName, InSequence.Num());

	for (int i = 0; i < InSequence.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s %d"), *InName, InSequence[i]);
	}
}