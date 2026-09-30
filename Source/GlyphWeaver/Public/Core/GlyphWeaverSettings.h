#pragma once

#include "CoreMinimal.h"
#include "GlyphWeaverLogger.h"
#include "Engine/DeveloperSettings.h"
#include "GlyphWeaverSettings.generated.h"

class UInputMappingContext;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnGlyphWeaverSettingsChanged, const FName&);

UCLASS(Config=Game, DefaultConfig, meta=(DisplayName="GlyphWeaver Settings"))
class GLYPHWEAVER_API UGlyphWeaverSettings : public UDeveloperSettings
{
	GENERATED_BODY()
	
public:
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif
	
	static FOnGlyphWeaverSettingsChanged OnSettingsChanged;
	
	void SetDebugEnabled(bool InIsDebugEnabled);
	void SetConsoleVerbosity(EGlyphWeaverConsoleVerbosity InIsConsoleVerbosity);
	
	bool GetDebugEnabled() const;
	EGlyphWeaverConsoleVerbosity GetConsoleVerbosity() const;
	
	UPROPERTY(Config, EditAnywhere, Category="Debug")
	bool DebugEnabled = false;
	
	UPROPERTY(Config, EditAnywhere, Category="Debug")
	EGlyphWeaverConsoleVerbosity ConsoleVerbosity = EGlyphWeaverConsoleVerbosity::Basic;
};