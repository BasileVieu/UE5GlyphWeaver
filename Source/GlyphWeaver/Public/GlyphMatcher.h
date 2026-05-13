#pragma once

#include "CoreMinimal.h"
#include "GlyphMatcher.generated.h"

struct FGlyphSequence;
class UGlyphPuzzleRule;

UCLASS()
class GLYPHWEAVER_API UGlyphMatcher : public UObject
{
	GENERATED_BODY()
	
public:
	bool Matches(FGlyphSequence& InTargetSequence, FGlyphSequence& InGuessSequence, int InModulo, TArray<UGlyphPuzzleRule*> InRules);
	void PrintSequence(const TArray<int>& InSequence, const FString& InName);
};