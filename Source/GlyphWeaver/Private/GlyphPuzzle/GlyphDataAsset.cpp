#include "GlyphPuzzle/GlyphDataAsset.h"

FGlyph UGlyphDataAsset::CreateGlyph() const
{
	return FGlyph(Name, Value);
}