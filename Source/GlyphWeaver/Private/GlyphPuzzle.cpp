#include "GlyphPuzzle.h"

FGlyph::FGlyph()
{
	Name = "";
	Value = 0;
}

FGlyph::FGlyph(FName InName, int InValue)
{
	Name = InName;
	Value = InValue;
}

bool FGlyph::operator==(const FGlyph& InOtherGlyph) const
{
	return Name == InOtherGlyph.Name
		&& Value == InOtherGlyph.Value;
}

void FGlyphSequence::Add(const FGlyph& InGlyph)
{
	GlyphSequence.Add(InGlyph);
}

void FGlyphSequence::Add(FName InName, int InValue)
{
	GlyphSequence.Add(FGlyph(InName, InValue));
}

void FGlyphSequence::RemoveFirst()
{
	if (Size() > 0)
	{
		GlyphSequence.RemoveAt(0);
	}
}

void FGlyphSequence::Empty()
{
	GlyphSequence.Empty();
}

FGlyph& FGlyphSequence::Get(const int InIndex)
{
	return GlyphSequence[InIndex];
}

int FGlyphSequence::Size() const
{
	return GlyphSequence.Num();
}