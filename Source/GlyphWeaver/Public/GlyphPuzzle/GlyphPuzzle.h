#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzle.generated.h"

class UGlyphPuzzleRule;

USTRUCT()
struct FGlyph
{
	GENERATED_BODY()
	
	FGlyph();
	explicit FGlyph(FName InName, int InValue);
	bool operator==(const FGlyph& InOtherGlyph) const;
	
	UPROPERTY(EditAnywhere)
	FName Name;
	
	UPROPERTY(EditAnywhere)
	int Value;
};

USTRUCT(Blueprintable)
struct FGlyphSequence
{
	GENERATED_BODY()
	
	void Add(const FGlyph& InGlyph);
	void Add(FName InName, int InValue);
	void RemoveFirst();
	void Empty();
	bool ContainsGlyph(const FGlyph& InGlyph) const;
	
	FGlyph& Get(int InIndex);
	
	int Size() const;
	
	TArray<FGlyph> Glyphs;
};

enum class EGlyphPuzzleState
{
	Inactive,
	Active,
	Solved
};

USTRUCT(Blueprintable)
struct FGlyphPuzzle
{
	GENERATED_BODY()
	
	FGlyphSequence Sequence;
	
	EGlyphPuzzleState State;
	
	UPROPERTY()
	TArray<UGlyphPuzzleRule*> Rules;
};