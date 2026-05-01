#include "GlyphMatcher.h"
#include "GlyphPuzzle.h"

bool UGlyphMatcher::Matches(FGlyphSequence& InTargetSequence, FGlyphSequence& InGuessSequence, int InModulo)
{
	FGlyphSequence InversedGuess = InGuessSequence;
	Algo::Reverse(InversedGuess.GlyphSequence);
	
	TArray<int> TargetDeltas = BuildDeltas(InTargetSequence, InModulo);
	
	TArray<int> GuessDeltas = BuildDeltas(InGuessSequence, InModulo);
	
	TArray<int> InversedGuessDeltas = BuildDeltas(InversedGuess, InModulo);
	
	TArray<int> CanonicalTarget = GetBestCanonicalSequence(TargetDeltas);
	
	TArray<int> CanonicalGuess = GetBestCanonicalSequence(GuessDeltas);
	
	TArray<int> CanonicalInversedGuess = GetBestCanonicalSequence(InversedGuessDeltas);

	return CanonicalTarget == CanonicalGuess
		|| CanonicalTarget == CanonicalInversedGuess;
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

TArray<int> UGlyphMatcher::GetBestCanonicalSequence(const TArray<int>& InSequence)
{
	int Length = InSequence.Num();

	TArray<int> Best = InSequence;

	for (int start = 1; start < Length; start++)
	{
		TArray<int> Candidate;

		for (int i = 0; i < Length; i++)
		{
			Candidate.Add(InSequence[(start + i) % Length]);
		}

		ECanonicalSequenceState CanonicalSequenceState = GetCanonicalSequenceState(Candidate, Best);

		if (CanonicalSequenceState == ECanonicalSequenceState::Smaller)
		{
			Best = Candidate;
		}
	}

	return Best;
}

ECanonicalSequenceState UGlyphMatcher::GetCanonicalSequenceState(const TArray<int>& InFirstSequence,
                                                                 const TArray<int>& InSecondSequence)
{
	if (InFirstSequence.Num() != InSecondSequence.Num())
	{
		return ECanonicalSequenceState::Error;
	}

	for (int i = 0; i < InFirstSequence.Num(); i++)
	{
		if (InFirstSequence[i] < InSecondSequence[i])
		{
			return ECanonicalSequenceState::Smaller;
		}

		if (InFirstSequence[i] > InSecondSequence[i])
		{
			return ECanonicalSequenceState::Bigger;
		}
	}

	return ECanonicalSequenceState::Equal;
}

void UGlyphMatcher::PrintSequence(const TArray<int>& InSequence, const FString& InName)
{
	UE_LOG(LogTemp, Warning, TEXT("%s count: %d\n"), *InName, InSequence.Num());

	for (int i = 0; i < InSequence.Num(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s %d"), *InName, InSequence[i]);
	}
}