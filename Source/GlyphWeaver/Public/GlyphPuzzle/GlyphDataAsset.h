#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzle.h"
#include "Engine/DataAsset.h"
#include "GlyphDataAsset.generated.h"

/**
 * Data Asset representing a glyph input definition.
 */
UCLASS()
class GLYPHWEAVER_API UGlyphDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	/**
	 * Creates and returns a FGlyph instance that use this asset's data.
	 * @return A FGlyph initialized with this asset's properties.
	 */
	FGlyph CreateGlyph() const;
	
private:
	/**
	 * Unique glyph name used to compare glyph equality.
	 */
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	FName GlyphName;

	/**
	 * Numeric value used by the matcher.
	 * Must be unique within in the same sequence.
	 */
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	int GlyphValue;
};