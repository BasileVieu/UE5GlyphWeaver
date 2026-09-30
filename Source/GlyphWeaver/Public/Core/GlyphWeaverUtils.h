#pragma once

#include "CoreMinimal.h"
#include "GlyphWeaverUtils.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGlyphWeaver, Display, All);

struct FGlyph;
struct FGlyphSequence;
struct FGlyphPuzzle;
class UGlyphPuzzleRule;

UCLASS()
class GLYPHWEAVER_API UGlyphWeaverUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	/**
	 * Checks developer settings' debug.
	 * @return True if debug is enabled in developer settings.
	 */
	static bool IsDebugEnabled();
	
	static FString GetIndent(int InDepth);
	
	/**
	 * Create and populate a string with glyph's data.
	 * @param InGlyph Glyph instance to transform into string.
	 * @param InIndent Indent to apply to structure the multilines.
	 * @return A string populated with glyph's data.
	 */
	UFUNCTION(BlueprintCallable)
	static FString GetGlyphString(FGlyph& InGlyph, int InIndent);

	/**
	 * Create and populate a string with sequence's data.
	 * @param InGlyphSequence Sequence instance to transform into string.
	 * @param InIndent Indent to apply to structure the multilines.
	 * @return A string populated with sequence's data.
	 */
	UFUNCTION(BlueprintCallable)
	static FString GetSequenceString(FGlyphSequence& InGlyphSequence, int InIndent);

	/**
	 * Create and populate a string with rule's data.
	 * @param InRule Rule pointer to transform into string.
	 * @param InIndent Indent to apply to structure the multilines.
	 * @return A string populated with rule's data.
	 */
	UFUNCTION(BlueprintCallable)
	static FString GetRuleString(UGlyphPuzzleRule* InRule, int InIndent);

	/**
	 * Create and populate a string with puzzle's data.
	 * @param InPuzzle Puzzle instance to transform into string.
	 * @param InIndent Indent to apply to structure the multilines.
	 * @return A string populated with puzzle's data.
	 */
	UFUNCTION(BlueprintCallable)
	static FString GetPuzzleString(FGlyphPuzzle& InPuzzle, int InIndent);
};