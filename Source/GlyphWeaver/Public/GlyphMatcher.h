#pragma once

#include "CoreMinimal.h"
#include "GlyphMatcher.generated.h"

struct FGlyphSequence;

enum class ECanonicalSequenceState : uint8
{
	Smaller = 0,
	Bigger = 1,
	Equal = 2,
	Error = 3
};

UCLASS()
class GLYPHWEAVER_API UGlyphMatcher : public UObject
{
	GENERATED_BODY()
	
public:
	bool Matches(FGlyphSequence& InTargetSequence, FGlyphSequence& InGuessSequence, int InModulo);
	TArray<int> BuildDeltas(FGlyphSequence& InSequence, int InModulo);
	int CircularDelta(FGlyphSequence& InSequence, int InIndex, int InModulo);
	TArray<int> GetBestCanonicalSequence(const TArray<int>& InSequence);
	ECanonicalSequenceState GetCanonicalSequenceState(const TArray<int>& InFirstSequence, const TArray<int>& InSecondSequence);
	void PrintSequence(const TArray<int>& InSequence, const FString& InName);
};