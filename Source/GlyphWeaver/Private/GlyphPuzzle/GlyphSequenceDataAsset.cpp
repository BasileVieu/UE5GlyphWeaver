#include "GlyphPuzzle/GlyphSequenceDataAsset.h"
#include "GlyphPuzzle/GlyphDataAsset.h"

FGlyphSequence UGlyphSequenceDataAsset::CreateGlyphSequence()
{
	FGlyphSequence Sequence;
	Sequence.Name = SequenceName;
	
	for (int i = 0; i < GlyphDataAssets.Num(); i++)
	{
		Sequence.Add(GlyphDataAssets[i]->CreateGlyph());
	}
	
	return Sequence;
}