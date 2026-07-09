using UnrealBuildTool;

public class GlyphWeaverEditor : ModuleRules
{
	public GlyphWeaverEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"SlateCore", "GlyphWeaver"
			});
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"EditorStyle",
				"UnrealEd",
				"LevelEditor",
				"ToolMenus",
				"PropertyEditor",
				"GlyphWeaver"
			}
		);
	}
}