using UnrealBuildTool;

public class ShipsClient : ModuleRules
{
	public ShipsClient(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core",
			"CoreUObject",
			"Engine",
			"UMG",
			"HTTP",
			"Json",
			"JsonUtilities",
			"ShipsOne"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}