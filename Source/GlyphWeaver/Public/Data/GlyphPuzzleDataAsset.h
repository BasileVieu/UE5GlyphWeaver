#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GlyphPuzzleDataAsset.generated.h"

class UInputMappingContext;
class UGlyphDataAsset;
class UInputAction;
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
	 * Time allowed to the player to add a new input.
	 */
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	float ResetTimer = 3.0f;
	
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
	
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	UInputMappingContext* GlyphInputMapping;
	
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	TMap<UInputAction*, TSoftObjectPtr<UGlyphDataAsset>> GlyphsInputMap;
}; 