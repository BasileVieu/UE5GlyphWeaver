#include "GlyphDataAsset.h"

FGlyph UGlyphDataAsset::CreateGlyph()
{
	return FGlyph(Name, Value);
}