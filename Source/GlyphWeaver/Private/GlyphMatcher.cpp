#include "GlyphMatcher.h"
#include "GlyphPuzzle.h"

bool UGlyphMatcher::Matches(FGlyphSequence& GlyphTargetSequence, FGlyphSequence& GlyphGuessSequence)
{
	TArray<int> NormalizedTargetSequence;
	TArray<int> NormalizedGuessSequence;
	
	for (int i = 1; i < GlyphTargetSequence.Size(); i++)
	{
		int Move = GlyphTargetSequence.CircularMove(i, i - 1, 4);
		
		if (Move != std::numeric_limits<int>::min())
		{
			NormalizedTargetSequence.Add(Move);
		}
	}
	
	for (int i = 1; i < GlyphGuessSequence.Size(); i++)
	{
		int Move = GlyphGuessSequence.CircularMove(i, i - 1, 4);
		
		if (Move != std::numeric_limits<int>::min())
		{
			NormalizedGuessSequence.Add(Move);
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Normalized Target Sequence count: %d\n"), NormalizedTargetSequence.Num());
	
	for (int i = 0; i < NormalizedTargetSequence.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Normalized Target Sequence %d"), NormalizedTargetSequence[i]);
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Normalized Guess Sequence count: %d\n"), NormalizedGuessSequence.Num());
	
	for (int i = 0; i < NormalizedGuessSequence.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Normalized Guess Sequence %d"), NormalizedGuessSequence[i]);
	}
	
	return true;
}