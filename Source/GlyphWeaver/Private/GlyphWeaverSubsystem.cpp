#include "GlyphWeaverSubsystem.h"
#include "GlyphMatcher.h"
#include "GlyphWeaver.h"
#include "GlyphWeaverUtils.h"
#include "GlyphPuzzle/GlyphPuzzleDataAsset.h"
#include "GlyphPuzzle/GlyphSequenceDataAsset.h"

void UGlyphWeaverSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	
	GuessGlyphSequence.Name = "Guess Sequence";
}

void UGlyphWeaverSubsystem::SetupPuzzle(const UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	GlyphPuzzle.Name = InPuzzleDataAsset->PuzzleName;
	UGlyphSequenceDataAsset* SequenceDataAsset = InPuzzleDataAsset->SequenceDataAsset.LoadSynchronous();
	GlyphPuzzle.Sequence = SequenceDataAsset->CreateGlyphSequence();
	GlyphPuzzle.State = EGlyphPuzzleState::Inactive;
	GlyphPuzzle.Rules = InPuzzleDataAsset->Rules;
	
	UGlyphWeaverUtils::PrintPuzzle(GlyphPuzzle);
}

void UGlyphWeaverSubsystem::AddGuessGlyphInput(FGlyph& InPlayerGlyph)
{
	UE_LOG(LogGlyphWeaver, Warning, TEXT("Add Player Glyph"));
	
	if (!GlyphPuzzle.Sequence.ContainsGlyph(InPlayerGlyph))
	{
		return;
	}
	
	OnGuessGlyphAdded.Broadcast(InPlayerGlyph);
	
	GuessGlyphSequence.Add(InPlayerGlyph);
	
	GetWorld()->GetTimerManager().SetTimer(UpdateGuessGlyphsTimerHandle,
		this, &UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs, 3.0f, true);
	
	UGlyphWeaverUtils::PrintSequence(GuessGlyphSequence);
	
	UE_LOG(LogGlyphWeaver, Warning, TEXT("Matches %s"),
		GlyphMatcher->Matches(GlyphPuzzle.Sequence, GuessGlyphSequence, 4, GlyphPuzzle.Rules)
		? TEXT("True") : TEXT("False"));
}

void UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs()
{
	UE_LOG(LogGlyphWeaver, Warning, TEXT("Remove Player Glyph"));
	
	OnGuessGlyphSequenceModified.Broadcast(GuessGlyphSequence);
	
	GuessGlyphSequence.Empty();
	
	GetWorld()->GetTimerManager().ClearTimer(UpdateGuessGlyphsTimerHandle);
	
	UGlyphWeaverUtils::PrintSequence(GuessGlyphSequence);
}

void UGlyphWeaverSubsystem::SetPause(bool InIsPaused)
{
	if (InIsPaused)
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdateGuessGlyphsTimerHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(UpdateGuessGlyphsTimerHandle,
			this, &UGlyphWeaverSubsystem::RemoveGuessGlyphsInputs, 3.0f, true);
	}
}