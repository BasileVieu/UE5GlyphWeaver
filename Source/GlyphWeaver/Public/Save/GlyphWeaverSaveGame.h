#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "GlyphWeaverSaveGame.generated.h"

UCLASS()
class GLYPHWEAVER_API UGlyphWeaverSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:
	UPROPERTY(SaveGame)
	TMap<FPrimaryAssetId, bool> PuzzlesSaved;
};