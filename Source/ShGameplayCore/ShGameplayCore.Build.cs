using UnrealBuildTool;

public class ShGameplayCore : ModuleRules
{
	public ShGameplayCore(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{ 
			"Core",
			"CoreUObject"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}