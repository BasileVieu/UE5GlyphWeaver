#include "GlyphPuzzle/GlyphPuzzleRuleReverse.h"
#include "Algo/Reverse.h"

FGlyphVariants UGlyphPuzzleRuleReverse::Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize)
{
	FGlyphVariants Results;
	
	for (const FGlyphVariant& Variant : InVariants)
	{
		Results.Variants.Add(Variant);
		
		FGlyphVariant Reversed = Variant;
		Algo::Reverse(Reversed.Values);
		
		Results.Variants.Add(Reversed);
	}
	
	return Results;
}