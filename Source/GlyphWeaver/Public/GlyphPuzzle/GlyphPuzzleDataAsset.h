#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GlyphPuzzleDataAsset.generated.h"

class UGlyphSequenceDataAsset;
class UGlyphPuzzleRule;

/**
 * Data asset defining a glyph puzzle configuration
 */
UCLASS()
class GLYPHWEAVER_API UGlyphPuzzleDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * Puzzle's unique name;
	 */
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	FName PuzzleName;
	
	/**
	 * Sequence data asset used as the puzzle solution.
	 */
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	TSoftObjectPtr<UGlyphSequenceDataAsset> SequenceDataAsset;

	/**
	 * Ordered rules applied to the puzzle validation logic.
	 */
	UPROPERTY(EditAnywhere, Instanced, Category="GlyphWeaver")
	TArray<TObjectPtr<UGlyphPuzzleRule>> Rules;
}; 