// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FpsU5CppV1 : ModuleRules
{
	public FpsU5CppV1(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "GameplayTasks", "NavigationSystem" });
	}
}
