#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzleRule.h"
#include "GlyphPuzzleRuleInputShift.generated.h"

/**
 * Rule example: Allowed player to validate the sequence with an offset in the inputs.
 */
UCLASS(Blueprintable, EditInlineNew)
class GLYPHWEAVER_API UGlyphPuzzleRuleInputShift : public UGlyphPuzzleRule
{
	GENERATED_BODY()

public:
	/**
	 * Create a sequence of int representing the offset between each input.
	 * @param InVariants Original variants to modify.
	 * @param InModulo Int mostly representing the max possible value of the glyphs.
	 * @param InTargetSize Int representing the size of the target sequence.
	 * @return A FGlyphVariants that contains collections of int representing the offset between each input.
	 */
	virtual FGlyphVariants Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize) override;

	/**
	 * Finds the closest offset from an input to the next one.
	 * Considers the first and last possible value are linked.
	 * @param InSequence Original sequence.
	 * @param InIndex Int used to find the offset between an element and the next one.
	 * @param InModulo Int mostly representing the max possible value of the glyphs.
	 * @return The offset between an element and the next one.
	 */
	int CircularDelta(const TArray<int>& InSequence, int InIndex, int InModulo);
};