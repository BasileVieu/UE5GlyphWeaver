#pragma once

#include "CoreMinimal.h"
#include "Puzzle/GlyphPuzzle.h"
#include "Engine/DataAsset.h"
#include "GlyphSequenceDataAsset.generated.h"

class UGlyphDataAsset;

/**
 * Data asset representing a glyph sequence definition.
 */
UCLASS()
class GLYPHWEAVER_API UGlyphSequenceDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * Creates and returns a FGlyphSequence instance that use this asset's data.
	 * @return A FGlyphSequence initialized with this asset's properties.
	 */
	FGlyphSequence CreateGlyphSequence();
	
private:
	/**
	 * Sequence's unique name.
	 */
	UPROPERTY(EditAnywhere)
	FName SequenceName;
	
	/**
	 * Collection of UGlyphDataAsset used to initialize the sequence instance.
	 */
	UPROPERTY(EditAnywhere)
	TArray<UGlyphDataAsset*> GlyphDataAssets;
};