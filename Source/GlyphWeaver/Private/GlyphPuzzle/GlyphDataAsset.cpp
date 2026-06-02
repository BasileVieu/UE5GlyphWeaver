#include "GlyphPuzzle/GlyphDataAsset.h"

FGlyph UGlyphDataAsset::CreateGlyph() const
{
	return FGlyph(GlyphName, GlyphValue);
}