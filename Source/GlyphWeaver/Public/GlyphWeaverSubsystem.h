#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzle/GlyphPuzzle.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlyphWeaverSubsystem.generated.h"

class UGlyphPuzzleDataAsset;
class UGlyphMatcher;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphEvent, FGlyph&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphSequenceEvent, FGlyphSequence&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphRuleEvent, UGlyphPuzzleRule*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphPuzzleEvent, FGlyphPuzzle&);

UCLASS()
class GLYPHWEAVER_API UGlyphWeaverSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	/**
	 * Initialize PuzzleGlyphSequence.
	 * @param InPuzzleDataAsset Original puzzle data asset.
	 */
	void SetupPuzzle(const UGlyphPuzzleDataAsset* InPuzzleDataAsset);

	/**
	 * Add a glyph to the guess sequence and begin/reset a timer to empty it.
	 * @param InPlayerGlyph Glyph to add to the sequence.
	 */
	void AddGuessGlyphInput(FGlyph& InPlayerGlyph);

	/**
	 * Empty guess sequence.
	 */
	void RemoveGuessGlyphsInputs();
	
	void SetPause(bool InIsPaused);
	
	FOnGlyphEvent OnGuessGlyphAdded;
	FOnGlyphSequenceEvent OnGuessGlyphSequenceModified;
	
private:
	FTimerHandle UpdateGuessGlyphsTimerHandle;
	
	UPROPERTY()
	FGlyphSequence GuessGlyphSequence;
	
	UPROPERTY()
	UGlyphMatcher* GlyphMatcher;
	
	FGlyphPuzzle GlyphPuzzle;
};