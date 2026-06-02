// Copyright Epic Games, Inc. All Rights Reserved.

#include "GlyphWeaver.h"

#define LOCTEXT_NAMESPACE "FGlyphWeaverModule"

DEFINE_LOG_CATEGORY(LogGlyphWeaver);
DEFINE_LOG_CATEGORY(LogGlyphWeaverDebug);

void FGlyphWeaverModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FGlyphWeaverModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FGlyphWeaverModule, GlyphWeaver)