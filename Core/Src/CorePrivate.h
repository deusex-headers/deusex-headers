/*=============================================================================
	CorePrivate.h: Unreal core private header file.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

/*----------------------------------------------------------------------------
	Core public includes.
----------------------------------------------------------------------------*/

#include "Core.h"

/*-----------------------------------------------------------------------------
	Locals functions.
-----------------------------------------------------------------------------*/

extern void appPlatformPreInit();
extern void appPlatformInit();
extern void appPlatformPreExit();
extern void appPlatformExit();

//extern UBOOL GNoGC;           // CoreI exports this.
//extern UBOOL GCheckConflicts; // CoreI exports this as GConflicts.
extern UBOOL GExitPurge;

/*-----------------------------------------------------------------------------
	Includes.
-----------------------------------------------------------------------------*/

#include "UnLinker.h"

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
