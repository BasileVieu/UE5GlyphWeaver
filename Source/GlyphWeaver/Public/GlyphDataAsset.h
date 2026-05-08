#pragma once

#include "CoreMinimal.h"
#include "GlyphSequence.h"
#include "Engine/DataAsset.h"
#include "GlyphDataAsset.generated.h"

UCLASS()
class GLYPHWEAVER_API UGlyphDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FGlyph CreateGlyph();
	
private:	
	UPROPERTY(EditAnywhere)
	FName Name;
	
	UPROPERTY(EditAnywhere)
	int Value;
};