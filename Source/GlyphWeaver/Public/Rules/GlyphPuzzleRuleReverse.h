#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzleRule.h"
#include "GlyphPuzzleRuleReverse.generated.h"

/**
 * Rule example: Allowed player to validate the sequence while trying the sequence in reverse.
 */
UCLASS(Blueprintable, EditInlineNew)
class GLYPHWEAVER_API UGlyphPuzzleRuleReverse : public UGlyphPuzzleRule
{
	GENERATED_BODY()
	
public:
	/**
	 * Reverse the original sequence and add it to the variants.
	 * @param InVariants Original variants to modify.
	 * @param InModulo Int mostly representing the max possible value of the glyphs.
	 * @param InTargetSize Int representing the size of the target sequence.
	 * @return A FGlyphVariants with the original sequence and the reversed one.
	 */
	virtual FGlyphVariants Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize) override;
};