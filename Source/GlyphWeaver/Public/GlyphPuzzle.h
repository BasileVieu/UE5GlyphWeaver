#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GlyphPuzzle.generated.h"

USTRUCT()
struct FGlyph
{
	GENERATED_BODY()
	
	FGlyph();
	explicit FGlyph(FName InName, int InValue);
	bool operator==(const FGlyph& InOtherGlyph) const;
	
	FName Name;
	int Value;
};

USTRUCT()
struct FGlyphSequence
{
	GENERATED_BODY()
	
	void Add(const FGlyph& InGlyph);
	void Add(FName InName, int InValue);
	void RemoveFirst();
	
	FGlyph& Get(int InIndex);
	int CircularMove(int InFirstIndex, int InSecondIndex, int InSequenceSize);
	
	int Size() const;
	
	TArray<FGlyph> GlyphSequence;
};