// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class murphys : ModuleRules
{
    private string ModulePath
    {
        get { return ModuleDirectory; }
    }

    private string ThirdPartyPath
    {
        get { return Path.GetFullPath(Path.Combine(ModulePath, "../../ThirdParty/")); }
    }

    public murphys(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam", "Slate", "SlateCore", "MoviePlayer", "Json", "Steamworks", "HTTP"});

        LoadOpenSSL(Target);
    }

    public bool LoadOpenSSL(ReadOnlyTargetRules Target)
    {
        bool isLibrarySupported = false;
        string Path = this.ThirdPartyPath;

        if ((Target.Platform == UnrealTargetPlatform.Win64) || (Target.Platform == UnrealTargetPlatform.Win32))
        {

            isLibrarySupported = true;

            string PlatformString = (Target.Platform == UnrealTargetPlatform.Win64) ? "x64" : "x86";
            string LibrariesPath = ThirdPartyPath + "/openssl/libraries";

            /*
            test your path with:
            using System; // Console.WriteLine("");
            Console.WriteLine("... LibrariesPath -> " + LibrariesPath);
            */

            PublicAdditionalLibraries.Add(LibrariesPath + "/openssl.lib");
            PublicAdditionalLibraries.Add(LibrariesPath + "/libssl.lib");
            PublicAdditionalLibraries.Add(LibrariesPath + "/libcrypto.lib");

        }

        if (isLibrarySupported)
        {
            // Include path
            PublicIncludePaths.Add(Path + "/openssl/includes");
        }

        PublicDefinitions.Add(string.Format("WITH_OPENSSL_BINDING={0}", isLibrarySupported ? 1 : 0));

        return isLibrarySupported;
    }
}
