// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Harris3D : ModuleRules
{
	public Harris3D(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "ProceduralMeshComponent", "MeshConversion" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay" });
	}
}
