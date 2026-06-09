#pragma once

#include "CoreMinimal.h"
#include "Puzzle/GlyphPuzzle.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlyphWeaverSubsystem.generated.h"

class UGlyphDataAsset;
struct FInputActionValue;
class UEnhancedInputLocalPlayerSubsystem;
class UGlyphPuzzleDataAsset;
class UGlyphMatcher;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphEvent, const FGlyph&);
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
	 * Initialize GlyphPuzzle.
	 * @param InPlayerController Controller triggering a puzzle activation.
	 * @param InPuzzleDataAsset Puzzle data asset used to initialize puzzle.
	 */
	void SetupPuzzle(APlayerController* InPlayerController, const UGlyphPuzzleDataAsset* InPuzzleDataAsset);

	/**
	 * Nullify cached enhanced input pointers and reset GlyphPuzzle if necessary.
	 * @param InPlayerController Controller leaving puzzle area.
	 */
	void RemovePuzzle(APlayerController* InPlayerController);

	/**
	 * Add a glyph to the guess sequence and begin/reset a timer to empty it.
	 * @param InPlayerGlyph Glyph to add to the sequence.
	 */
	void AddGuessGlyphInput(const FGlyph& InPlayerGlyph);

	/**
	 * Empty guess sequence.
	 */
	void RemoveGuessGlyphsInputs();
	
	void SetPause(bool InIsPaused);
	
	FOnGlyphEvent OnGuessGlyphAdded;
	FOnGlyphSequenceEvent OnGuessGlyphSequenceModified;
	
private:
	/**
	 * Funciton used as lambda to bind all puzzle inputs to the correct values.
	 * @param InGlyphDataAsset Glyph data asset used to create glyph instance.
	 * @param InValue Corresponding puzzle input value.
	 */
	void PlayerInputTriggered(UGlyphDataAsset* InGlyphDataAsset, const FInputActionValue& InValue);
	
	FTimerHandle UpdateGuessGlyphsTimerHandle;
	
	UPROPERTY()
	FGlyphSequence GuessGlyphSequence;
	
	UPROPERTY()
	UGlyphMatcher* GlyphMatcher;
	
	UPROPERTY()
	FGlyphPuzzle GlyphPuzzle;
	
	UPROPERTY()
	UEnhancedInputLocalPlayerSubsystem* CachedEnhancedSubsystem;
	
	UPROPERTY()
	UEnhancedInputComponent* CachedEnhancedComponent;
	
	UPROPERTY()
	TArray<int> CachedEnhancedBindings;
};