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

FGlyph& FGlyphSequence::Get(const int InIndex)
{
	return GlyphSequence[InIndex];
}

int FGlyphSequence::CircularMove(int InFirstIndex, int InSecondIndex, int InSequenceSize)
{
	if (InFirstIndex < 0
		|| InFirstIndex >= Size()
		|| InSecondIndex < 0
		|| InSecondIndex >= Size())
	{
		return std::numeric_limits<int>::min();
	}
	
	int FirstValue = Get(InFirstIndex).Value;
	int SecondValue = Get(InSecondIndex).Value;
	
	int Diff = FirstValue - SecondValue;
	int AbsDiff = FMath::Abs(Diff);
	
	return FMath::Min(AbsDiff, InSequenceSize - AbsDiff) * (Diff > 0 ? 1 : -1);
}

int FGlyphSequence::Size() const
{
	return GlyphSequence.Num();
}