#pragma once

#include "CoreMinimal.h"
#include "GlyphSequence.h"
#include "Engine/DataAsset.h"
#include "GlyphSequenceDataAsset.generated.h"

class UGlyphDataAsset;

UCLASS()
class GLYPHWEAVER_API UGlyphSequenceDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FGlyphSequence CreateGlyphSequence();
	
private:
	UPROPERTY(EditAnywhere)
	TArray<UGlyphDataAsset*> GlyphDataAssets;
};