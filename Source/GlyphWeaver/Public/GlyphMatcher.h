#pragma once

#include "CoreMinimal.h"
#include "GlyphMatcher.generated.h"

struct FGlyphSequence;

UCLASS()
class GLYPHWEAVER_API UGlyphMatcher : public UObject
{
	GENERATED_BODY()
	
public:
	bool Matches(FGlyphSequence& GlyphTargetSequence, FGlyphSequence& GlyphGuessSequence);
};