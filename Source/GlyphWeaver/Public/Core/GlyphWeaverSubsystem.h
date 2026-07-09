#pragma once

#include "CoreMinimal.h"
#include "Puzzle/GlyphPuzzle.h"
#include "Puzzle/GlyphPuzzleActor.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlyphWeaverSubsystem.generated.h"

struct FPuzzleData;
class UGlyphWeaverSaveGame;
class UGlyphDataAsset;
struct FInputActionValue;
class UEnhancedInputLocalPlayerSubsystem;
class UGlyphPuzzleDataAsset;
class UGlyphMatcher;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphEvent, const FGlyph&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphSequenceEvent, const FGlyphSequence&);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphRuleEvent, UGlyphPuzzleRule*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnPuzzleDataEvent, const FPuzzleData&);
DECLARE_MULTICAST_DELEGATE(FOnEmptyEvent);

USTRUCT()
struct FPuzzleData
{
	GENERATED_BODY()
	
	FPuzzleData()
	{
		Puzzle = {};
		Actor = nullptr;
		Solved = false;
	}
	
	FPuzzleData(const FPrimaryAssetId& InAssetId, FGlyphPuzzle InPuzzle, AGlyphPuzzleActor* InActor, bool InSolved)
	{
		AssetId = InAssetId;
		Puzzle = InPuzzle;
		Actor = InActor;
		Solved = InSolved;
	}
	
	FPrimaryAssetId AssetId;
	
	FGlyphPuzzle Puzzle;
	
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

	static FGlyphPuzzle CreatePuzzle(const UGlyphPuzzleDataAsset* InPuzzleDataAsset);
	
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
	
	void ValidatePuzzle(const FPrimaryAssetId& InPuzzleAssetId);
	
	void ResetPuzzle(const FPrimaryAssetId& InPuzzleAssetId);
	
	const TMap<FPrimaryAssetId, FPuzzleData>& GetPuzzles() const;
	
	const FPuzzleData* GetCurrentPuzzleData() const;
	
	UFUNCTION(BlueprintCallable)
	void ResetAllPuzzles();
	
	UFUNCTION(BlueprintCallable)
	void Save();
	
	UFUNCTION(BlueprintCallable)
	void Load();
	
	FOnGlyphEvent OnGuessGlyphAdded;
	FOnGlyphSequenceEvent OnGuessGlyphSequenceModified;
	FOnPuzzleDataEvent OnRegisteredPuzzle;
	FOnEmptyEvent OnCurrentPuzzleDataChanged;
	
private:
	void InitializePuzzleActor(AGlyphPuzzleActor* InActor, const FPuzzleData* InPuzzleData);
	
	/**
	 * Funciton used as lambda to bind all puzzle inputs to the correct values.
	 * @param InGlyphDataAsset Glyph data asset used to create glyph instance.
	 * @param InValue Corresponding puzzle input value.
	 */
	void PlayerInputTriggered(const UGlyphDataAsset* InGlyphDataAsset, const FInputActionValue& InValue);
	
	void ValidateCurrentPuzzle();
	
	void RetrieveSaveGameData();
	
	void ApplySaveGameData();
	
	bool IsPuzzleSolved(const FPuzzleData* InPuzzleData);
	
	FTimerHandle CurrentTimerHandle;
	
	UPROPERTY()
	FGlyphSequence GuessGlyphSequence;
	
	UPROPERTY()
	TObjectPtr<UGlyphMatcher> GlyphMatcher;
	
	FPuzzleData* CurrentPuzzleData;
	
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
	TMap<FPrimaryAssetId, FPuzzleData> PuzzleDatas;
	
	UPROPERTY()
	TObjectPtr<UGlyphWeaverSaveGame> SaveGame;
};