// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class GamesDev_Assignment2 : ModuleRules
{
	public GamesDev_Assignment2(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate",
			"NavigationSystem"
        });

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"GamesDev_Assignment2",
			"GamesDev_Assignment2/Variant_Platforming",
			"GamesDev_Assignment2/Variant_Platforming/Animation",
			"GamesDev_Assignment2/Variant_Combat",
			"GamesDev_Assignment2/Variant_Combat/AI",
			"GamesDev_Assignment2/Variant_Combat/Animation",
			"GamesDev_Assignment2/Variant_Combat/Gameplay",
			"GamesDev_Assignment2/Variant_Combat/Interfaces",
			"GamesDev_Assignment2/Variant_Combat/UI",
			"GamesDev_Assignment2/Variant_SideScrolling",
			"GamesDev_Assignment2/Variant_SideScrolling/AI",
			"GamesDev_Assignment2/Variant_SideScrolling/Gameplay",
			"GamesDev_Assignment2/Variant_SideScrolling/Interfaces",
			"GamesDev_Assignment2/Variant_SideScrolling/UI"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
