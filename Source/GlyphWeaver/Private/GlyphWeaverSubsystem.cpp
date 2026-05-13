#include "GlyphWeaverSubsystem.h"
#include "GlyphMatcher.h"
#include "GlyphPuzzle/GlyphPuzzleDataAsset.h"
#include "GlyphPuzzle/GlyphSequenceDataAsset.h"

void UGlyphWeaverSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGlyphWeaverSubsystem::SetupPuzzle(UGlyphPuzzleDataAsset* InPuzzleDataAsset)
{
	UGlyphSequenceDataAsset* SequenceDataAsset = InPuzzleDataAsset->SequenceDataAsset.LoadSynchronous();
	GlyphPuzzle.Sequence = SequenceDataAsset->CreateGlyphSequence();
	GlyphPuzzle.State = EGlyphPuzzleState::Inactive;
	GlyphPuzzle.Rules = InPuzzleDataAsset->Rules;
	
	PrintGlyphsSequence(GlyphPuzzle.Sequence);
}

void UGlyphWeaverSubsystem::SetPause(bool InIsPaused)
{
	if (InIsPaused)
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdatePlayerGlyphsTimerHandle);
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(UpdatePlayerGlyphsTimerHandle,
			this, &UGlyphWeaverSubsystem::RemovePlayerGlyphsInputs, 3.0f, true);
	}
}

void UGlyphWeaverSubsystem::AddPlayerGlyphInput(const FGlyph& InPlayerGlyph)
{
	UE_LOG(LogTemp, Warning, TEXT("Add Player Glyph"));
	
	if (!GlyphPuzzle.Sequence.ContainsGlyph(InPlayerGlyph))
	{
		return;
	}
	
	PlayerGlyphSequence.Add(InPlayerGlyph);
	
	GetWorld()->GetTimerManager().SetTimer(UpdatePlayerGlyphsTimerHandle,
		this, &UGlyphWeaverSubsystem::RemovePlayerGlyphsInputs, 3.0f, true);
	
	PrintGlyphsSequence(PlayerGlyphSequence);
	
	UE_LOG(LogTemp, Warning, TEXT("Matches %s"), GlyphMatcher->Matches(GlyphPuzzle.Sequence, PlayerGlyphSequence, 4, GlyphPuzzle.Rules)
		? TEXT("True") : TEXT("False"));
}

void UGlyphWeaverSubsystem::RemovePlayerGlyphsInputs()
{
	UE_LOG(LogTemp, Warning, TEXT("Remove Player Glyph"));
	
	PlayerGlyphSequence.Empty();
	
	GetWorld()->GetTimerManager().ClearTimer(UpdatePlayerGlyphsTimerHandle);
	
	PrintGlyphsSequence(PlayerGlyphSequence);
}

void UGlyphWeaverSubsystem::PrintGlyphsSequence(FGlyphSequence& GlyphSequenceToPrint)
{
	UE_LOG(LogTemp, Warning, TEXT("Glyphs count: %d\n"), GlyphSequenceToPrint.Size());
	
	for (int i = 0; i < GlyphSequenceToPrint.Size(); i++)
	{
		UE_LOG(LogTemp, Warning, TEXT("Glyph %d: %s"), i, *GlyphSequenceToPrint.Get(i).Name.ToString());
	}
}