#include "GlyphSequenceDataAsset.h"
#include "GlyphDataAsset.h"

FGlyphSequence UGlyphSequenceDataAsset::CreateGlyphSequence()
{
	FGlyphSequence Sequence;
	
	for (int i = 0; i < GlyphDataAssets.Num(); i++)
	{
		Sequence.Add(GlyphDataAssets[i]->CreateGlyph());
	}
	
	return Sequence;
}