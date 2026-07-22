using UnrealBuildTool;

public class ShipsServer : ModuleRules
{
	public ShipsServer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"NetCore",
			"ShGameplayCore",
			"ShipsOne"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });
	}
}