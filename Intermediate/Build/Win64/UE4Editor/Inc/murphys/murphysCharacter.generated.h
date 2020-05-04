// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
#ifdef MURPHYS_murphysCharacter_generated_h
#error "murphysCharacter.generated.h already included, missing '#pragma once' in murphysCharacter.h"
#endif
#define MURPHYS_murphysCharacter_generated_h

#define murphys_Source_murphys_murphysCharacter_h_12_SPARSE_DATA
#define murphys_Source_murphys_murphysCharacter_h_12_RPC_WRAPPERS
#define murphys_Source_murphys_murphysCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS
#define murphys_Source_murphys_murphysCharacter_h_12_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesAmurphysCharacter(); \
	friend struct Z_Construct_UClass_AmurphysCharacter_Statics; \
public: \
	DECLARE_CLASS(AmurphysCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/murphys"), NO_API) \
	DECLARE_SERIALIZER(AmurphysCharacter)


#define murphys_Source_murphys_murphysCharacter_h_12_INCLASS \
private: \
	static void StaticRegisterNativesAmurphysCharacter(); \
	friend struct Z_Construct_UClass_AmurphysCharacter_Statics; \
public: \
	DECLARE_CLASS(AmurphysCharacter, ACharacter, COMPILED_IN_FLAGS(0 | CLASS_Config), CASTCLASS_None, TEXT("/Script/murphys"), NO_API) \
	DECLARE_SERIALIZER(AmurphysCharacter)


#define murphys_Source_murphys_murphysCharacter_h_12_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API AmurphysCharacter(const FObjectInitializer& ObjectInitializer); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(AmurphysCharacter) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AmurphysCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AmurphysCharacter); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AmurphysCharacter(AmurphysCharacter&&); \
	NO_API AmurphysCharacter(const AmurphysCharacter&); \
public:


#define murphys_Source_murphys_murphysCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API AmurphysCharacter(AmurphysCharacter&&); \
	NO_API AmurphysCharacter(const AmurphysCharacter&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, AmurphysCharacter); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(AmurphysCharacter); \
	DEFINE_DEFAULT_CONSTRUCTOR_CALL(AmurphysCharacter)


#define murphys_Source_murphys_murphysCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	FORCEINLINE static uint32 __PPO__CameraBoom() { return STRUCT_OFFSET(AmurphysCharacter, CameraBoom); } \
	FORCEINLINE static uint32 __PPO__FollowCamera() { return STRUCT_OFFSET(AmurphysCharacter, FollowCamera); }


#define murphys_Source_murphys_murphysCharacter_h_9_PROLOG
#define murphys_Source_murphys_murphysCharacter_h_12_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	murphys_Source_murphys_murphysCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	murphys_Source_murphys_murphysCharacter_h_12_SPARSE_DATA \
	murphys_Source_murphys_murphysCharacter_h_12_RPC_WRAPPERS \
	murphys_Source_murphys_murphysCharacter_h_12_INCLASS \
	murphys_Source_murphys_murphysCharacter_h_12_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define murphys_Source_murphys_murphysCharacter_h_12_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	murphys_Source_murphys_murphysCharacter_h_12_PRIVATE_PROPERTY_OFFSET \
	murphys_Source_murphys_murphysCharacter_h_12_SPARSE_DATA \
	murphys_Source_murphys_murphysCharacter_h_12_RPC_WRAPPERS_NO_PURE_DECLS \
	murphys_Source_murphys_murphysCharacter_h_12_INCLASS_NO_PURE_DECLS \
	murphys_Source_murphys_murphysCharacter_h_12_ENHANCED_CONSTRUCTORS \
private: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> MURPHYS_API UClass* StaticClass<class AmurphysCharacter>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID murphys_Source_murphys_murphysCharacter_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
