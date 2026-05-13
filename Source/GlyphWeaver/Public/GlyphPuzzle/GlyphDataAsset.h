#pragma once

#include "CoreMinimal.h"
#include "GlyphPuzzle.h"
#include "Engine/DataAsset.h"
#include "GlyphDataAsset.generated.h"

UCLASS()
class GLYPHWEAVER_API UGlyphDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FGlyph CreateGlyph() const;
	
private:	
	UPROPERTY(EditAnywhere)
	FName Name;
	
	UPROPERTY(EditAnywhere)
	int Value;
};