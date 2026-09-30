#include "GlyphWeaverEditorModule.h"
#include "Editor.h"
#include "GlyphWeaverEditorTab.h"
#include "Core/GlyphWeaverSettings.h"
#include "ToolMenus.h"
#include "Framework/Docking/TabManager.h"
#include "Style/GlyphWeaverEditorSlateStyle.h"
#include "Widgets/Docking/SDockTab.h"

DEFINE_LOG_CATEGORY(LogGlyphWeaverDebug);

IMPLEMENT_MODULE(FGlyphWeaverEditorModule, GlyphWeaverEditor)

const FName FGlyphWeaverEditorModule::GlyphWeaverEditorTabName("GlyphWeaverDebug");

void FGlyphWeaverEditorModule::StartupModule()
{
	FGlyphWeaverEditorSlateStyle::Initialize();

	FGlobalTabmanager::Get()->RegisterNomadTabSpawner(GlyphWeaverEditorTabName,
	                                                  FOnSpawnTab::CreateRaw(this, &FGlyphWeaverEditorModule::OnSpawnTab))
	                        .SetDisplayName(FText::FromString("Debug GlyphWeaver"))
	                        .SetMenuType(ETabSpawnerMenuType::Hidden);

	UToolMenus::RegisterStartupCallback(FSimpleMulticastDelegate::FDelegate::CreateRaw(this, &FGlyphWeaverEditorModule::RegisterMenus));
	
	FEditorDelegates::BeginPIE.AddRaw(this, &FGlyphWeaverEditorModule::OnBeginPIE);
}

void FGlyphWeaverEditorModule::ShutdownModule()
{
	FGlyphWeaverEditorSlateStyle::Shutdown();

	UToolMenus::UnRegisterStartupCallback(this);
	UToolMenus::UnregisterOwner(this);
	
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner(GlyphWeaverEditorTabName);
	
	FEditorDelegates::EndPIE.RemoveAll(this);
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

		                   SubSection.AddMenuEntry("OpenDebugGlyphWeaver",
		                                           FText::FromString("DebugGlyphWeaver"),
		                                           FText::FromString(TEXT("Ouvre la fenêtre DebugGlyphWeaver")),
		                                           FSlateIcon(),
		                                           FUIAction(FExecuteAction::CreateStatic(
			                                           &FGlyphWeaverEditorModule::OpenTab)));

		                   SubSection.AddMenuEntry("GlyphWeaverDebugEnable",
		                                           FText::FromString("Enable GlyphWeaver Debug"),
		                                           FText::FromString("Enable or disable GlyphWeaver Debug"),
		                                           FSlateIcon(),
		                                           FUIAction(
			                                           FExecuteAction::CreateRaw(this, &FGlyphWeaverEditorModule::OnToggleDebugEnable),
			                                           FCanExecuteAction(),
			                                           FIsActionChecked::CreateRaw(this, &FGlyphWeaverEditorModule::IsDebugEnabled)),
			                                           EUserInterfaceActionType::ToggleButton);

		                   SubSection.AddSubMenu("GlyphWeaverConsoleVerbosity",
		                                         FText::FromString("GlyphWeaver Log Console Verbosity"),
		                                         FText::FromString("Change GlyphWeaver log verbosity in console"),
		                                         FNewMenuDelegate::CreateLambda([this](FMenuBuilder& MenuBuilder)
		                                         {
			                                         for (uint8 Index = 0; Index <= static_cast<uint8>(EGlyphWeaverConsoleVerbosity::Trace); ++Index)
			                                         {
				                                         const auto Verbosity = static_cast<EGlyphWeaverConsoleVerbosity>(Index);

				                                         MenuBuilder.AddMenuEntry(UEnum::GetDisplayValueAsText(Verbosity),
					                                         FText::GetEmpty(),
					                                         FSlateIcon(),
					                                         FUIAction(FExecuteAction::CreateLambda([this, Verbosity]
					                                                   {
						                                                   SetConsoleVerbosity(Verbosity);
					                                                   }),
					                                                   FCanExecuteAction(),
					                                                   FIsActionChecked::CreateLambda([this, Verbosity]
					                                                   {
						                                                   return GetConsoleVerbosity() == Verbosity;
					                                                   })),
					                                         NAME_None,
					                                         EUserInterfaceActionType::RadioButton);
			                                         }
		                                         }));
	                   }));
}

void FGlyphWeaverEditorModule::OnBeginPIE(const bool InIsSimulating)
{
	
}

void FGlyphWeaverEditorModule::OnToggleDebugEnable()
{
	UGlyphWeaverSettings* Settings = GetMutableDefault<UGlyphWeaverSettings>();
	
	Settings->DebugEnabled = !Settings->DebugEnabled;
}

void FGlyphWeaverEditorModule::SetConsoleVerbosity(const EGlyphWeaverConsoleVerbosity InConsoleVerbosity)
{
	UGlyphWeaverSettings* Settings = GetMutableDefault<UGlyphWeaverSettings>();
	
	Settings->ConsoleVerbosity = InConsoleVerbosity;
}

void FGlyphWeaverEditorModule::OpenTab()
{
	FGlobalTabmanager::Get()->TryInvokeTab(GlyphWeaverEditorTabName);
}

bool FGlyphWeaverEditorModule::IsDebugEnabled() const
{
	return GetDefault<UGlyphWeaverSettings>()->GetDebugEnabled();
}

EGlyphWeaverConsoleVerbosity FGlyphWeaverEditorModule::GetConsoleVerbosity() const
{
	return GetDefault<UGlyphWeaverSettings>()->GetConsoleVerbosity();
}

TSharedRef<SDockTab> FGlyphWeaverEditorModule::OnSpawnTab(const FSpawnTabArgs& InSpawnTabArgs)
{
	TSharedRef<SGlyphWeaverEditorTab> TabContent = SNew(SGlyphWeaverEditorTab);
	
	return SNew(SDockTab)
		.TabRole(NomadTab)
		[
			SNew(SOverlay)
			+ SOverlay::Slot()
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Top)
			[
				SNew(SBox)
				[
					TabContent
				]
			]
		];
}
