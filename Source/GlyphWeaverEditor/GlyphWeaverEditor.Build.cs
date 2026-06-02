using UnrealBuildTool;

public class GlyphWeaverEditor : ModuleRules
{
	public GlyphWeaverEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core"
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"GlyphWeaver",
				"Slate",
				"SlateCore",
				"UnrealEd",
				"ToolMenus",
				"PropertyEditor",
				"LevelEditor",
				"EditorStyle"
			}
		);
	}
}