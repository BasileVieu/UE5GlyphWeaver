#include "Puzzle/GlyphPuzzle.h"

FGlyph::FGlyph()
{
	Name = "";
	Value = 0;
}

FGlyph::FGlyph(const FName InName, const int InValue)
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
	Glyphs.Add(InGlyph);
}

void FGlyphSequence::Add(const FName InName, const int InValue)
{
	Glyphs.Add(FGlyph(InName, InValue));
}

void FGlyphSequence::RemoveFirst()
{
	if (Size() > 0)
	{
		Glyphs.RemoveAt(0);
	}
}

void FGlyphSequence::Empty()
{
	Glyphs.Empty();
}

bool FGlyphSequence::ContainsGlyph(const FGlyph& InGlyph) const
{
	for (int i = 0; i < Glyphs.Num(); i++)
	{
		if (Glyphs[i] == InGlyph)
		{
			return true;
		}
	}
	
	return false;
}

FGlyph& FGlyphSequence::Get(const int InIndex)
{
	return Glyphs[InIndex];
}

int FGlyphSequence::Size() const
{
	return Glyphs.Num();
}