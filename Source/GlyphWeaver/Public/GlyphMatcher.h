#pragma once

#include "CoreMinimal.h"
#include "GlyphMatcher.generated.h"

class UGlyphSequenceDataAsset;
struct FGlyphSequence;

UCLASS()
class GLYPHWEAVER_API UGlyphMatcher : public UObject
{
	GENERATED_BODY()
	
public:
	bool Matches(FGlyphSequence& InTargetSequence, FGlyphSequence& InGuessSequence, int InModulo);
	bool MatchDeltas(const TArray<int>& InFirstSequence, const TArray<int>& InSecondSequence);
	TArray<int> BuildDeltas(FGlyphSequence& InSequence, int InModulo);
	int CircularDelta(FGlyphSequence& InSequence, int InIndex, int InModulo);
	void PrintSequence(const TArray<int>& InSequence, const FString& InName);
	
private:
	UPROPERTY()
	UGlyphSequenceDataAsset* TargetSequenceDataSequence;
};