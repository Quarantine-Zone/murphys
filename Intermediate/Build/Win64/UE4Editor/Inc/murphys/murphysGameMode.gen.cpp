// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "murphys/murphysGameMode.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodemurphysGameMode() {}
// Cross Module References
	MURPHYS_API UClass* Z_Construct_UClass_AmurphysGameMode_NoRegister();
	MURPHYS_API UClass* Z_Construct_UClass_AmurphysGameMode();
	ENGINE_API UClass* Z_Construct_UClass_AGameModeBase();
	UPackage* Z_Construct_UPackage__Script_murphys();
// End Cross Module References
	void AmurphysGameMode::StaticRegisterNativesAmurphysGameMode()
	{
	}
	UClass* Z_Construct_UClass_AmurphysGameMode_NoRegister()
	{
		return AmurphysGameMode::StaticClass();
	}
	struct Z_Construct_UClass_AmurphysGameMode_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_AmurphysGameMode_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_AGameModeBase,
		(UObject* (*)())Z_Construct_UPackage__Script_murphys,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_AmurphysGameMode_Statics::Class_MetaDataParams[] = {
		{ "HideCategories", "Info Rendering MovementReplication Replication Actor Input Movement Collision Rendering Utilities|Transformation" },
		{ "IncludePath", "murphysGameMode.h" },
		{ "ModuleRelativePath", "murphysGameMode.h" },
		{ "ShowCategories", "Input|MouseInput Input|TouchInput" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_AmurphysGameMode_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<AmurphysGameMode>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_AmurphysGameMode_Statics::ClassParams = {
		&AmurphysGameMode::StaticClass,
		"Game",
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		0,
		0,
		0x008802ACu,
		METADATA_PARAMS(Z_Construct_UClass_AmurphysGameMode_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_AmurphysGameMode_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_AmurphysGameMode()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_AmurphysGameMode_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(AmurphysGameMode, 2223427819);
	template<> MURPHYS_API UClass* StaticClass<AmurphysGameMode>()
	{
		return AmurphysGameMode::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_AmurphysGameMode(Z_Construct_UClass_AmurphysGameMode, &AmurphysGameMode::StaticClass, TEXT("/Script/murphys"), TEXT("AmurphysGameMode"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(AmurphysGameMode);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
