using UnrealBuildTool;

public class ShipsOneServerTarget : TargetRules
{
    public ShipsOneServerTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Server;
        DefaultBuildSettings = BuildSettingsVersion.V4;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_5;
        ExtraModuleNames.Add("ShipsOne");
    }
}