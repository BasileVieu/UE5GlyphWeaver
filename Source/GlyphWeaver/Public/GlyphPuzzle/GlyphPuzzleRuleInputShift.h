#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzleRule.h"
#include "GlyphPuzzleRuleInputShift.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class GLYPHWEAVER_API UGlyphPuzzleRuleInputShift : public UGlyphPuzzleRule
{
	GENERATED_BODY()

public:
	virtual FGlyphVariants Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize) override;

	int CircularDelta(const TArray<int>& InSequence, int InIndex, int InModulo);
};