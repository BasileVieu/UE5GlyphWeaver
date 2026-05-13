#include "GlyphPuzzle/GlyphPuzzleRuleSequenceShift.h"

FGlyphVariants UGlyphPuzzleRuleSequenceShift::Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize)
{
	FGlyphVariants Results;
	
	for (const FGlyphVariant& Variant : InVariants)
	{
		FGlyphVariant Extended = Variant;
		Extended.Values.Append(Variant.Values);
		
		for (int start = 0; start < Extended.Values.Num() - InTargetSize; start++)
		{
			FGlyphVariant NewVariant;
			
			for (int i = 0; i < InTargetSize; i++)
			{
				NewVariant.Values.Add(Extended.Values[start + i]);
			}
			
			Results.Variants.Add(NewVariant);
		}
	}
	
	return Results;
}