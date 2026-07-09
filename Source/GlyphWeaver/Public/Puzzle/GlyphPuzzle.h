#pragma once

#include "CoreMinimal.h"
#include "UObject/PrimaryAssetId.h"
#include "GlyphPuzzle.generated.h"

class UGlyphPuzzleRule;

/**
 * Represents a single glyph with a unique name and associated value.
 */
USTRUCT(Blueprintable)
struct FGlyph
{
	GENERATED_BODY()
	
	FGlyph();

	/**
	 * Creates a glyph with the given name and value.
	 * @param InName Name identifying the glyph.
	 * @param InValue Numeric value associated with the glyph.
	 */
	explicit FGlyph(FName InName, int InValue);

	/**
	 * Checks whether two glyphs are identical.
	 * @param InOtherGlyph Glyph to compare against.
	 * @return True if both glyphs share the same name and value.
	 */
	bool operator==(const FGlyph& InOtherGlyph) const;

	/**
	 * Unique glyph name used for identification and comparison.
	 */
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	FName Name;

	/**
	 * Numeric value associated with the glyph.
	 */
	UPROPERTY(EditAnywhere, Category="GlyphWeaver")
	int Value;
};

/**
 * Represents an ordered collection of glyphs.
 */
USTRUCT(Blueprintable)
struct FGlyphSequence
{
	GENERATED_BODY()

	/**
	 * Adds a glyph to the sequence.
	 * @param InGlyph Glyph to add.
	 */
	void Add(const FGlyph& InGlyph);

	/**
	 * Creates and adds a glyph to the sequence.
	 * @param InName Name of the glyph.
	 * @param InValue Balue of the glyph.
	 */
	void Add(FName InName, int InValue);

	/**
	 * Removes the first glyph of the sequence.
	 */
	void RemoveFirst();

	/**
	 * Removes all glyphs from the sequence.
	 */
	void Empty();

	/**
	 * Checks whether the sequence contains a specific glyph.
	 * @param InGlyph Glyph to search for.
	 * @return True if the glyph exists in the sequence.
	 */
	bool ContainsGlyph(const FGlyph& InGlyph) const;

	/**
	 * Returns the glyph at the specific index.
	 * @param InIndex Index of the glyph to retrieve.
	 * @return Reference to the glyph at the given index.
	 */
	FGlyph& Get(int InIndex);
	
	int GetMaxValue() const;

	/**
	 * Returns the number of glyphs in the sequence.
	 * @return Total glyph count.
	 */
	int Size() const;

	/**
	 * Unique name.
	 */
	FName Name;

	/**
	 * Ordered list of glyphs composing the sequence.
	 */
	TArray<FGlyph> Glyphs;
};

/**
 * Contains the current sequence, state and rules of a glyph puzzle.
 */
USTRUCT(Blueprintable)
struct FGlyphPuzzle
{
	GENERATED_BODY()

	/**
	 * Unique id to identify the corresponding asset data.
	 */
	FPrimaryAssetId PrimaryAssetId;

	/**
	 * Puzzle's unique name;
	 */
	FName Name;

	/**
	 * Current glyph sequence associated with the puzzle.
	 */
	FGlyphSequence Sequence;

	/**
	 * Current puzzle state.
	 */
	bool Solved;

	/**
	 * Rules applied to the puzzle validation logic.
	 */
	UPROPERTY()
	TArray<UGlyphPuzzleRule*> Rules;
};