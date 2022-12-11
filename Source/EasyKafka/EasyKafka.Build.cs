// Copyright (c) 2022-2023 MrShaaban, Mohamad Shaaban, https://github.com/sha3sha3/UE-EasyKafka.

using System;
using System.IO;
using UnrealBuildTool;

public class EasyKafka : ModuleRules
{
	public EasyKafka(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		CppStandard = CppStandardVersion.Cpp17;

		PublicIncludePaths.AddRange(
			new string[] {
				// ... add public include paths required here ...
				Path.Combine(PluginDirectory,"Source/KafkaConsumer/Public"),
                Path.Combine(PluginDirectory,"Source/KafkaProducer/Public")
            }
			);


		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);


		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
				"CoreUObject"
				// ... add other public dependencies that you statically link with here ...
			}
			);


		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"Engine",
				"KafkaLib",
				"KafkaConsumer",
				"KafkaProducer"
				// ... add private dependencies that you statically link with here ...	
			}
			);
		bEnableExceptions = true;
		bUseRTTI = true;
	}
}
