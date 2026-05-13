#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzleRule.generated.h"

USTRUCT(BlueprintType)
struct FGlyphVariant
{
	GENERATED_BODY()	
	
	FGlyphVariant() = default;
	explicit FGlyphVariant(const TArray<int>& InValues);
	
	bool operator==(const FGlyphVariant& Element) const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int> Values;
};

USTRUCT(BlueprintType)
struct FGlyphVariants
{
	GENERATED_BODY()
	
	FGlyphVariants() = default;
	explicit FGlyphVariants(const FGlyphVariant& InVariant);
	explicit FGlyphVariants(const TArray<int>& InValues);
	
	auto begin() { return Variants.begin(); }
	auto end() { return Variants.end(); }
	
	auto begin() const { return Variants.begin(); }
	auto end() const { return Variants.end(); }
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FGlyphVariant> Variants;
};

UCLASS(Abstract, Blueprintable, EditInlineNew, DefaultToInstanced)
class GLYPHWEAVER_API UGlyphPuzzleRule : public UObject
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintNativeEvent)
	FGlyphVariants Apply(const FGlyphVariants& InVariants, int InModulo, int InTargetSize);
	
	virtual FGlyphVariants Apply_CPP(const FGlyphVariants& InVariants, int InModulo, int InTargetSize);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ApplyToTarget = false;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ApplyToGuess = false;
};