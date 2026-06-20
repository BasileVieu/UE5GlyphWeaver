#pragma once

#include "CoreMinimal.h"
#include "Puzzle/GlyphPuzzle.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlyphWeaverSubsystem.generated.h"

class UGlyphWeaverSaveGame;
class AGlyphPuzzleActor;
class UGlyphDataAsset;
struct FInputActionValue;
class UEnhancedInputLocalPlayerSubsystem;
class UGlyphPuzzleDataAsset;
class UGlyphMatcher;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphEvent, const FGlyph&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphSequenceEvent, FGlyphSequence&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphRuleEvent, UGlyphPuzzleRule*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphPuzzleEvent, FGlyphPuzzle&);

USTRUCT()
struct FPuzzleData
{
	GENERATED_BODY()
	
	FPuzzleData()
	{
		Actor = nullptr;
		Solved = false;
	}
	
	FPuzzleData(AGlyphPuzzleActor* InActor, bool InSolved)
	{
		Actor = InActor;
		Solved = InSolved;
	}
	
	TWeakObjectPtr<AGlyphPuzzleActor> Actor;
	
	bool Solved;
};

UCLASS()
class GLYPHWEAVER_API UGlyphWeaverSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	
	void RegisterPuzzleActor(AGlyphPuzzleActor* InActor, const UGlyphPuzzleDataAsset* InPuzzleDataAsset);
	
	FGlyphPuzzle CreatePuzzle(const UGlyphPuzzleDataAsset* InPuzzleDataAsset);
	
	/**
	 * Initialize GlyphPuzzle.
	 * @param InPlayerController Controller triggering a puzzle activation.
	 * @param InPuzzleDataAsset Puzzle data asset used to initialize puzzle.
	 */
	void DetectPuzzle(const APlayerController* InPlayerController, const UGlyphPuzzleDataAsset* InPuzzleDataAsset);

	/**
	 * Nullify cached enhanced input pointers and reset GlyphPuzzle if necessary.
	 * @param InPlayerController Controller leaving puzzle area.
	 */
	void UnDetectPuzzle(const APlayerController* InPlayerController);

	/**
	 * Add a glyph to the guess sequence and begin/reset a timer to empty it.
	 * @param InPlayerGlyph Glyph to add to the sequence.
	 */
	void AddGuessGlyphInput(const FGlyph& InPlayerGlyph);

	/**
	 * Empty guess sequence.
	 */
	void RemoveGuessGlyphsInputs();

	/**
	 * Pause/Unpause the current puzzle timer.
	 * @param InIsPaused The pause state to apply.
	 */
	void PausePuzzleTimer(bool InIsPaused) const;
	
	void ValidatePuzzle(FPrimaryAssetId InPrimaryAssetId);
	
	void ResetPuzzle(FPrimaryAssetId InPrimaryAssetId);
	
	UFUNCTION(BlueprintCallable)
	void ResetAllPuzzles();
	
	UFUNCTION(BlueprintCallable)
	void Save();
	
	UFUNCTION(BlueprintCallable)
	void Load();
	
	FOnGlyphEvent OnGuessGlyphAdded;
	FOnGlyphSequenceEvent OnGuessGlyphSequenceModified;
	
private:
	void InitializePuzzleActor(AGlyphPuzzleActor* InActor, const UGlyphPuzzleDataAsset* InPuzzleDataAsset);
	
	/**
	 * Funciton used as lambda to bind all puzzle inputs to the correct values.
	 * @param InGlyphDataAsset Glyph data asset used to create glyph instance.
	 * @param InValue Corresponding puzzle input value.
	 */
	void PlayerInputTriggered(const UGlyphDataAsset* InGlyphDataAsset, const FInputActionValue& InValue);
	
	void ValidateCurrentPuzzle();
	
	void RetrieveSaveGameData();
	
	void ApplySaveGameData();
	
	bool IsPuzzleSolved(const FGlyphPuzzle& InPuzzle);
	
	bool IsPuzzleSolved(const UGlyphPuzzleDataAsset* InPuzzleDataAsset);
	
	FTimerHandle CurrentTimerHandle;
	
	UPROPERTY()
	FGlyphSequence GuessGlyphSequence;
	
	UPROPERTY()
	TObjectPtr<UGlyphMatcher> GlyphMatcher;
	
	UPROPERTY()
	FGlyphPuzzle CurrentPuzzle;
	
	UPROPERTY()
	float CurrentResetTimer;
	
	UPROPERTY()
	TWeakObjectPtr<UWorld> CachedWorld;
	
	UPROPERTY()
	TWeakObjectPtr<UEnhancedInputLocalPlayerSubsystem> CachedEnhancedSubsystem;
	
	UPROPERTY()
	TWeakObjectPtr<UEnhancedInputComponent> CachedEnhancedComponent;
	
	UPROPERTY()
	TArray<int> CachedEnhancedBindings;
	
	UPROPERTY()
	TMap<FPrimaryAssetId, FPuzzleData> Puzzles;
	
	UPROPERTY()
	TObjectPtr<UGlyphWeaverSaveGame> SaveGame;
};