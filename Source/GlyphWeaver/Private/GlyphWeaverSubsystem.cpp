#include "GlyphWeaverSubsystem.h"
#include "GlyphMatcher.h"
#include "GlyphSequence.h"

void UGlyphWeaverSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
}

void UGlyphWeaverSubsystem::SetupPuzzle(const FGlyphSequence& InPuzzleSequence)
{
	PuzzleGlyphSequence = InPuzzleSequence;
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
	
	PlayerGlyphSequence.Add(InPlayerGlyph);
	
	GetWorld()->GetTimerManager().SetTimer(UpdatePlayerGlyphsTimerHandle,
		this, &UGlyphWeaverSubsystem::RemovePlayerGlyphsInputs, 3.0f, true);
	
	PrintGlyphsSequence(PlayerGlyphSequence);
	
	UE_LOG(LogTemp, Warning, TEXT("Matches %s"), GlyphMatcher->Matches(PuzzleGlyphSequence, PlayerGlyphSequence, 4)
		? TEXT("True") : TEXT("False"));
}

void UGlyphWeaverSubsystem::RemovePlayerGlyphsInputs()
{
	UE_LOG(LogTemp, Warning, TEXT("Remove Player Glyph"));
	
	PlayerGlyphSequence.Empty();
	
	if (PlayerGlyphSequence.Size() == 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(UpdatePlayerGlyphsTimerHandle);
	}
	
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