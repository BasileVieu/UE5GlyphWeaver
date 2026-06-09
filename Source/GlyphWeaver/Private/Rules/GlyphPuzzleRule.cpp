#include "Rules/GlyphPuzzleRule.h"

FGlyphVariant::FGlyphVariant(const TArray<int>& InValues)
{
	Values = InValues;
}

FGlyphVariants::FGlyphVariants(const FGlyphVariant& InVariant)
{
	Variants = { InVariant };
}

FGlyphVariants::FGlyphVariants(const TArray<int>& InValues)
{
	Variants = { FGlyphVariant(InValues) };
}

FGlyphVariants UGlyphPuzzleRule::Apply_Implementation(const FGlyphVariants& InVariants, int InModulo, int InTargetSize)
{
	return Apply_CPP(InVariants, InModulo, InTargetSize);
}

FGlyphVariants UGlyphPuzzleRule::Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize)
{
	return InVariants;
}

bool FGlyphVariant::operator==(const FGlyphVariant& Element) const
{
	if (Values.Num() != Element.Values.Num())
	{
		return false;
	}
	
	for (int i = 0; i < Values.Num(); i++)
	{
		if (Values[i] != Element.Values[i])
		{
			return false;
		}
	}
	
	return true;
}