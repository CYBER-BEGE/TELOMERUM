// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class TELOMERUM : ModuleRules
{
	public TELOMERUM(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"UMG",
			"Slate",
			"SlateCore",
			"NavigationSystem"
		});

		PrivateDependencyModuleNames.AddRange(new string[] {  });

        PublicIncludePaths.AddRange(new string[] {
            "TELOMERUM",
			"TELOMERUM/Player",
            "TELOMERUM/Enemy",
            "TELOMERUM/AI",
            "TELOMERUM/Interfaces",
            "TELOMERUM/UI",
            "TelOMERUM/Item",
            "TELOMERUM/Trap",
        });

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
