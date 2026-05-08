#include "GlyphMatcher.h"
#include "GlyphSequence.h"

bool UGlyphMatcher::Matches(FGlyphSequence& InTargetSequence, FGlyphSequence& InGuessSequence, int InModulo)
{
	if (InTargetSequence.Size() > InGuessSequence.Size())
	{
		return false;
	}
	
	TArray<int> DeltasTarget = BuildDeltas(InTargetSequence, InModulo);
	
	//FGlyphSequence InversedGuess = InGuessSequence;
	//Algo::Reverse(InversedGuess.GlyphSequence);
	
	for (int i = 0; i <= InGuessSequence.Size() - InTargetSequence.Size(); i++)
	{
		FGlyphSequence ReducedSequence;
		
		for (int j = 0; j < InTargetSequence.Size(); j++)
		{
			ReducedSequence.Add(InGuessSequence.Get(i + j));
		}
		
		TArray<int> ReducedDeltas = BuildDeltas(ReducedSequence, InModulo);
		
		if (MatchDeltas(ReducedDeltas, DeltasTarget))
		{
			return true;
		}
	}
	
	return false;
}

bool UGlyphMatcher::MatchDeltas(const TArray<int>& InSequence, const TArray<int>& InSubSequence)
{
	TArray<int> ExtendedSequence = InSequence;
	ExtendedSequence.Append(InSequence);
	
	for (int start = 0; start < ExtendedSequence.Num() - InSubSequence.Num(); start++)
	{
		bool Match = true;
		
		for (int i = 0; i < InSubSequence.Num(); i++)
		{
			/*UE_LOG(LogTemp, Warning, TEXT("start: %d, i: %d, start + i: %d, Delta Target: %d, Extended Deltas Guess: %d"),
				start, i, start + i, InSubSequence[i], ExtendedSequence[start + i]);*/
			
			if (InSubSequence[i] != ExtendedSequence[start + i])
			{
				Match = false;
				
				break;
			}
		}
		
		if (Match)
		{
			return true;
		}
	}
	
	return false;
}

TArray<int> UGlyphMatcher::BuildDeltas(FGlyphSequence& InSequence, int InModulo)
{
	TArray<int> Result;
	
	for (int i = 0; i < InSequence.Size(); i++)
	{
		Result.Add(CircularDelta(InSequence, i, InModulo));
	}
	
	return Result;
}

int UGlyphMatcher::CircularDelta(FGlyphSequence& InSequence, int InIndex, int InModulo)
{
	int FirstValue = InSequence.Get(InIndex).Value;
	int SecondValue = InSequence.Get((InIndex + 1) % InSequence.Size()).Value;
	
	int Forward = (SecondValue - FirstValue + InModulo) % InModulo;
	int Backward = (FirstValue - SecondValue + InModulo) % InModulo;
	
	return Forward <= Backward ? Forward : -Backward;
}

void UGlyphMatcher::PrintSequence(const TArray<int>& InSequence, const FString& InName)
{
	UE_LOG(LogTemp, Warning, TEXT("%s count: %d\n"), *InName, InSequence.Num());

	for (int i = 0; i < InSequence.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s %d"), *InName, InSequence[i]);
	}
}