#include "Rules/GlyphPuzzleRuleInputShift.h"

FGlyphVariants UGlyphPuzzleRuleInputShift::Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize)
{
	FGlyphVariants Results;
	
	for (const FGlyphVariant& Variant : InVariants)
	{
		FGlyphVariant Result;
	
		for (int i = 0; i < Variant.Values.Num(); i++)
		{
			Result.Values.Add(CircularDelta(Variant.Values, i, InModulo));
		}
		
		Results.Variants.Add(Result);
	}
	
	return Results;
}

int UGlyphPuzzleRuleInputShift::CircularDelta(const TArray<int>& InSequence, int InIndex, int InModulo)
{
	int FirstValue = InSequence[InIndex];
	int SecondValue = InSequence[(InIndex + 1) % InSequence.Num()];
	
	int Forward = (SecondValue - FirstValue + InModulo) % InModulo;
	int Backward = (FirstValue - SecondValue + InModulo) % InModulo;
	
	return Forward <= Backward ? Forward : -Backward;
}