/*=============================================================================
	UnObjVer.h: Unreal object version.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

/*-----------------------------------------------------------------------------
	Version coding.
-----------------------------------------------------------------------------*/

// Earliest engine build that is network compatible with this one.
#define ENGINE_MIN_NET_VERSION 1100

// Engine build number, for displaying to end users.
#define ENGINE_VERSION 1100

// Base protocol version to negotiate in network play.
#define ENGINE_NEGOTIATION_VERSION 1100

// Prevents incorrect files from being loaded.
#define PACKAGE_FILE_TAG 0x9E2A83C1

// The current Unrealfile version.
#define PACKAGE_FILE_VERSION 68

// The earliest file version which we can load with complete
// backwards compatibility. Must be at least PACKAGE_FILE_VERSION.
#define PACKAGE_MIN_VERSION 60

// Defines for easier building of multi game projects. --han
#define DEFAULT_PACKAGE TEXT("DeusEx")
#define DEUS_EX         1
#define LAUNCH_BAG      1

// Engine features and build options.
//#define NODECALS 1 // Only define if really no decals.
//#define UED2 1     // Only define if really UnrealEd 2.

// Type used for UViewport::CurrentTime  and variables used
// in combination with it such as UTexture::LastUpdateTime.
typedef DOUBLE VIEWPORTTIME;

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
