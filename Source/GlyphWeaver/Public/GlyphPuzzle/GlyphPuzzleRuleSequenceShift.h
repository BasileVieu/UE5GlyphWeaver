#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzleRule.h"
#include "GlyphPuzzleRuleSequenceShift.generated.h"

UCLASS(Blueprintable, EditInlineNew)
class GLYPHWEAVER_API UGlyphPuzzleRuleSequenceShift : public UGlyphPuzzleRule
{
	GENERATED_BODY()
	
public:
	virtual FGlyphVariants Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize) override;
};