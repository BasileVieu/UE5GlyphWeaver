#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzle/GlyphPuzzle.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlyphWeaverSubsystem.generated.h"

class UGlyphPuzzleDataAsset;
class UGlyphMatcher;

UCLASS()
class GLYPHWEAVER_API UGlyphWeaverSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	/**
	 * Initialize PuzzleGlyphSequence.
	 * @param InPuzzleDataAsset Original Puzzle
	 */
	UFUNCTION(BlueprintCallable, Category="GlyphWeaver", meta=(ToolTip="Initialize PuzzleGlyphSequence"))
	void SetupPuzzle(UGlyphPuzzleDataAsset* InPuzzleDataAsset);
	void SetPause(bool InIsPaused);
	void AddPlayerGlyphInput(const FGlyph& InPlayerGlyph);
	void RemovePlayerGlyphsInputs();
	void PrintGlyphsSequence(FGlyphSequence& GlyphsToPrint);
	
private:
	FTimerHandle UpdatePlayerGlyphsTimerHandle;
	
	UPROPERTY()
	FGlyphSequence PlayerGlyphSequence;
	
	UPROPERTY()
	UGlyphMatcher* GlyphMatcher;
	
	FGlyphPuzzle GlyphPuzzle;
};