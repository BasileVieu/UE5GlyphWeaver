#pragma once

#include "CoreMinimal.h"
#include "GlyphMatcher.generated.h"

struct FGlyphSequence;
class UGlyphPuzzleRule;

/**
 * Class applying rules to target and guess sequences to create variants.
 * Then checks if one of the target variant and the guess variants matches.
 */
UCLASS()
class GLYPHWEAVER_API UGlyphMatcher : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	 * Creates target and guess variants and checks if there is an equality between them.
	 * @param InTargetSequence Original target sequence.
	 * @param InGuessSequence Original guess sequence.
	 * @param InModulo Int mostly representing the max possible value of the glyphs.
	 * @param InRules Collection of rules applied to the sequences.
	 * @return True if one of the target variant is equal to one of the guess sequence.
	 */
	bool Matches(FGlyphSequence& InTargetSequence, FGlyphSequence& InGuessSequence, int InModulo, TArray<UGlyphPuzzleRule*> InRules);
	
	void PrintSequence(const TArray<int>& InSequence, const FString& InName);
};