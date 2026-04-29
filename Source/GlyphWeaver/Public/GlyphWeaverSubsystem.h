#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzle.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "GlyphWeaverSubsystem.generated.h"

class UGlyphMatcher;

UCLASS()
class GLYPHWEAVER_API UGlyphWeaverSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
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