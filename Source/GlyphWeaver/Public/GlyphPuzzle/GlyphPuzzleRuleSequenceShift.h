#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzleRule.h"
#include "GlyphPuzzleRuleSequenceShift.generated.h"

/**
 * Rule example: Allowed player to validate the sequence while beginning int the middle of the target sequence.
 */
UCLASS(Blueprintable, EditInlineNew)
class GLYPHWEAVER_API UGlyphPuzzleRuleSequenceShift : public UGlyphPuzzleRule
{
	GENERATED_BODY()
	
public:
	/**
	 * Creates a temporary sequence containing twice the original one.
	 * Then returns collections of variants the same size of the target sequence.
	 * @param InVariants Original variants to modify.
	 * @param InModulo Int mostly representing the max possible value of the glyphs.
	 * @param InTargetSize Int representing the size of the target sequence.
	 * @return Collections of variants the same size of the target sequence.
	 */
	virtual FGlyphVariants Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize) override;
};