// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "Modules/ModuleManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGlyphWeaver, Log, All);
DECLARE_LOG_CATEGORY_EXTERN(LogGlyphWeaverDebug, Verbose, All);

class FGlyphWeaverModule : public IModuleInterface
{
public:

	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
};