#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzleRule.generated.h"

/**
 * Custom struct containing an array of values (int) representing a variant of the original sequence.
 */
USTRUCT(BlueprintType)
struct FGlyphVariant
{
	GENERATED_BODY()
	
	FGlyphVariant() = default;
	explicit FGlyphVariant(const TArray<int>& InValues);

	/**
	 * Function used to check equality with another variant.
	 * @param Element Other variant to check equality.
	 * @return True if all values are equal within the variants.
	 */
	bool operator==(const FGlyphVariant& Element) const;

	/**
	 * Ordered collection of int representing values of the variant.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> Values;
};

/**
 * Custom struct containing an array of FGlyphVariant representing all variants of the same sequence.
 */
USTRUCT(BlueprintType)
struct FGlyphVariants
{
	GENERATED_BODY()
	
	FGlyphVariants() = default;
	explicit FGlyphVariants(const FGlyphVariant& InVariant);
	explicit FGlyphVariants(const TArray<int>& InValues);	
	
	// Functions used for loop
	auto begin() { return Variants.begin(); }
	auto end() { return Variants.end(); }
	
	auto begin() const { return Variants.begin(); }
	auto end() const { return Variants.end(); }

	/**
	 * Ordered collection of FGlyphVariant representing all variants of the same sequence.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGlyphVariant> Variants;
};

/**
 * Represents a rule used by the matcher.
 */
UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class GLYPHWEAVER_API UGlyphPuzzleRule : public UObject
{
	GENERATED_BODY()
	
public:
	/**
	 * Rule's logic applied by the matcher. Must be modified only in blueprints.
	 * @param InVariants Original variants to modify.
	 * @param InModulo Int mostly representing the max possible value of the glyphs.
	 * @param InTargetSize Int representing the size of the target sequence.
	 * @return A FGlyphVariants that contains the new variants used by the matcher.
	 */
	UFUNCTION(BlueprintNativeEvent)
	FGlyphVariants Apply(const FGlyphVariants& InVariants, int InModulo, int InTargetSize);
	
	/**
	 * Rule's logic applied by the matcher. Must be modified only in cpp.
	 * @param InVariants Original variants to modify.
	 * @param InModulo Int mostly representing the max possible value of the glyphs.
	 * @param InTargetSize Int representing the size of the target sequence.
	 * @return A FGlyphVariants that contains the new variants used by the matcher.
	 */
	virtual FGlyphVariants Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize);

	/**
	 * Boolean used to know if the rule's logic must be applied to the target sequence and variants.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ApplyToTarget = false;
	
	/**
	 * Boolean used to know if the rule's logic must be applied to the guess sequence and variants.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ApplyToGuess = false;
};