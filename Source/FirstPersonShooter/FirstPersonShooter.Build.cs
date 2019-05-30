// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using System;
using System.IO;
using UnrealBuildTool;

public class FirstPersonShooter : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public FirstPersonShooter(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "AIModule", "GameplayTasks" });

        // Adding libusb library
        LoadStaticLibrary(Target);


    }

    public bool LoadStaticLibrary(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {
            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = Path.Combine(ThirdPartyPath, "UE4_staticLibrary", "Libraries");

            /*
            test your path with:
            using System; 
            
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */

            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "UE4_staticLibrary." + PlatformString + ".lib"));
            PublicAdditionalLibraries.Add(Path.Combine(LibrariesPath, "libusb-1.0" + ".lib"));

            Console.WriteLine("Added successfully libusb library ");

        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path.Combine(ThirdPartyPath, "UE4_staticLibrary", "Includes"));
        }

        Definitions.Add(string.Format("WITH_BOBS_MAGIC_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}