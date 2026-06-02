#pragma once

#include "CoreMinimal.h"
#include "GlyphWeaverUtils.generated.h"

struct FGlyph;
struct FGlyphSequence;
struct FGlyphPuzzle;
class UGlyphPuzzleRule;

UCLASS()
class GLYPHWEAVER_API UGlyphWeaverUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static bool IsDebugEnabled();
	
	static FString GetIndent(int InDepth);
	
	static void PrintMultipleLines(const FString& InString);
	
	UFUNCTION(BlueprintCallable)
	static void PrintGlyph(FGlyph& InGlyph);
	
	UFUNCTION(BlueprintCallable)
	static void PrintSequence(FGlyphSequence& InGlyphSequence);
	
	UFUNCTION(BlueprintCallable)
	static void PrintRule(UGlyphPuzzleRule* InRule);
	
	UFUNCTION(BlueprintCallable)
	static void PrintPuzzle(FGlyphPuzzle& InPuzzle);
	
	UFUNCTION(BlueprintCallable)
	static FString GetGlyphString(FGlyph& InGlyph, int InIndent);
	
	UFUNCTION(BlueprintCallable)
	static FString GetSequenceString(FGlyphSequence& InGlyphSequence, int InIndent);
	
	UFUNCTION(BlueprintCallable)
	static FString GetRuleString(UGlyphPuzzleRule* InRule, int InIndent);
	
	UFUNCTION(BlueprintCallable)
	static FString GetPuzzleString(FGlyphPuzzle& InPuzzle, int InIndent);
};