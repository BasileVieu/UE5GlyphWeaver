#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "GlyphWeaverSettings.generated.h"

class UInputMappingContext;

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="GlyphWeaver Settings"))
class GLYPHWEAVER_API UGlyphWeaverSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
	UPROPERTY(Config, EditAnywhere, Category="Debug")
	bool DebugEnabled = false;
};