#pragma once

#include "CoreMinimal.h"
#include "GlyphSequence.generated.h"

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
	
	FGlyph& Get(int InIndex);
	
	int Size() const;
	
	TArray<FGlyph> GlyphSequence;
};