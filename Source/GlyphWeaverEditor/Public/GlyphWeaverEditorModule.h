#pragma once

#include "Modules/ModuleInterface.h"
#include "Modules/ModuleManager.h"

class FGlyphWeaverEditorModule : public IModuleInterface
{
public:
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	
private:
	void RegisterMenus();
	void OnToggleDebugEnable();
	
	bool IsDebugEnabled() const;
};