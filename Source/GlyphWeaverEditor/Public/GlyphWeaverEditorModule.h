#pragma once

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
	
	TSharedRef<SDockTab> OnSpawnTab(const FSpawnTabArgs& InSpawnTabArgs);
	
	bool IsDebugEnabled() const;
	
	TSharedPtr<SGlyphWeaverEditorTab> GlyphWeaverEditorTab;
	
	static const FName GlyphWeaverEditorTabName;
};
