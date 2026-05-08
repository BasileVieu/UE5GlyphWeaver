#pragma once

#include "CoreMinimal.h"
#include "GlyphSequence.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlyphWeaverSubsystem.generated.h"

class UGlyphSequenceDataAsset;
class UGlyphMatcher;

UCLASS()
class GLYPHWEAVER_API UGlyphWeaverSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	/**
	 * Initialize PuzzleGlyphSequence.
	 * @param InPuzzleSequence Original Puzzle Sequence
	 */
	UFUNCTION(BlueprintCallable, Category="GlyphWeaver", meta=(ToolTip="Initialize PuzzleGlyphSequence"))
	void SetupPuzzle(const FGlyphSequence& InPuzzleSequence);
	void SetPause(bool InIsPaused);
	void AddPlayerGlyphInput(const FGlyph& InPlayerGlyph);
	void RemovePlayerGlyphsInputs();
	void PrintGlyphsSequence(FGlyphSequence& GlyphsToPrint);
	
private:
	FTimerHandle UpdatePlayerGlyphsTimerHandle;
	
	UPROPERTY()
	FGlyphSequence PuzzleGlyphSequence;
	
	UPROPERTY()
	FGlyphSequence PlayerGlyphSequence;
	
	UPROPERTY()
	UGlyphMatcher* GlyphMatcher;
};