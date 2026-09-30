#pragma once

#include "Core/GlyphWeaverLogger.h"
#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"
#include "Framework/Docking/TabManager.h"

DECLARE_LOG_CATEGORY_EXTERN(LogGlyphWeaverDebug, Verbose, All);

class SGlyphWeaverEditorTab;

class FGlyphWeaverEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	static void OpenTab();
	
	void RegisterMenus();
	void OnBeginPIE(const bool InIsSimulating);
	void OnToggleDebugEnable();
	void SetConsoleVerbosity(EGlyphWeaverConsoleVerbosity InConsoleVerbosity);
	
	TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& InSpawnTabArgs);
	
	bool IsDebugEnabled() const;
	EGlyphWeaverConsoleVerbosity GetConsoleVerbosity() const;
	
	TSharedPtr<SGlyphWeaverEditorTab> GlyphWeaverEditorTab;
	
	static const FName GlyphWeaverEditorTabName;
};
