#include "GlyphWeaverEditorModule.h"

#include "GlyphWeaverSettings.h"
#include "ToolMenus.h"

IMPLEMENT_MODULE(FGlyphWeaverEditorModule, GlyphWeaverEditor)

void FGlyphWeaverEditorModule::StartupModule()
{
	UE_LOG(LogTemp, Log, TEXT("GlyphWeaverEditor loaded"));

	UToolMenus::RegisterStartupCallback(
		FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGlyphWeaverEditorModule::RegisterMenus));
}

void FGlyphWeaverEditorModule::ShutdownModule()
{
	UE_LOG(LogTemp, Log, TEXT("GlyphWeaverEditor unloaded"));

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
}

void FGlyphWeaverEditorModule::RegisterMenus()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.MainMenu");

	if (!Menu)
	{
		return;
	}

	FToolMenuSection& Section = Menu->AddSection("GlyphWeaverDebugSection",
	                                             FText::FromString("GlyphWeaverDebugSection"));

	Section.AddSubMenu("GlyphWeaverDebugSubMenu",
	                   FText::FromString("GlyphWeaver"),
	                   FText::FromString("Menu GlyphWeaver"),
	                   FNewToolMenuDelegate::CreateLambda([this](UToolMenu* InSubMenu)
	                   {
		                   FToolMenuSection& SubSection = InSubMenu->AddSection("GlyphWeaverDebug", FText::GetEmpty());

		                   SubSection.AddMenuEntry("GlyphWeaverDebugEnable",
		                                           FText::FromString("Enable GlyphWeaver Debug"),
		                                           FText::FromString("Enable or disable GlyphWeaver Debug"),
		                                           FSlateIcon(),
		                                           FUIAction(
			                                           FExecuteAction::CreateRaw(this, &FGlyphWeaverEditorModule::OnToggleDebugEnable),
			                                           FCanExecuteAction(),
			                                           FIsActionChecked::CreateRaw(this, &FGlyphWeaverEditorModule::IsDebugEnabled)),
		                                           EUserInterfaceActionType::ToggleButton);
	                   }));
}

void FGlyphWeaverEditorModule::OnToggleDebugEnable()
{
	UGlyphWeaverSettings* Settings = GetMutableDefault<UGlyphWeaverSettings>();
	
	Settings->DebugEnabled = !Settings->DebugEnabled;
}

bool FGlyphWeaverEditorModule::IsDebugEnabled() const
{
	return GetDefault<UGlyphWeaverSettings>()->DebugEnabled;
}