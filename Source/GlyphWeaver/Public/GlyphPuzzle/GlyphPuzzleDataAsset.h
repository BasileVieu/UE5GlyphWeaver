#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GlyphPuzzleDataAsset.generated.h"

class UGlyphSequenceDataAsset;
class UGlyphPuzzleRule;

UCLASS()
class GLYPHWEAVER_API UGlyphPuzzleDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<UGlyphSequenceDataAsset> SequenceDataAsset;
	
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UGlyphPuzzleRule>> Rules;
};