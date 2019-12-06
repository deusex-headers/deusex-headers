/*=============================================================================
	UnFactory.h: Factory class definition.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*----------------------------------------------------------------------------
	UFactory.
----------------------------------------------------------------------------*/

//
// An object responsible for creating and importing new objects.
//
class CORE_API UFactory : public UObject
{
	DECLARE_ABSTRACT_CLASS(UFactory,UObject,0)

	// Friends.
	#include "Friends/UFactory.h"

	// Per-class variables.
	UClass*         SupportedClass;
	UClass*			ContextClass;
	FString			Description;
	FString			InContextCommand;
	FString			OutOfContextCommand;
	TArray<FString> Formats;
	BITFIELD        bCreateNew         : 1;
	BITFIELD		bShowPropertySheet : 1;
	BITFIELD		bShowCategories    : 1;
	BITFIELD		bText              : 1;
	BITFIELD		bMulti			   : 1;
	INT				AutoPriority;

	// Constructors.
	UFactory();
	void StaticConstructor();

	// UObject interface.
	void Serialize( FArchive& Ar );

	// UFactory interface.
	virtual UObject* FactoryCreateText( UClass* InClass, UObject* InOuter, FName InName, DWORD InFlags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn ) {return NULL;}
	virtual UObject* FactoryCreateBinary( UClass* InClass, UObject* InOuter, FName InName, DWORD InFlags, UObject* Context, const TCHAR* Type, const BYTE*& Buffer, const BYTE* BufferEnd, FFeedbackContext* Warn ) {return NULL;}
	virtual UObject* FactoryCreateNew( UClass* InClass, UObject* InOuter, FName InName, DWORD InFlags, UObject* Context, FFeedbackContext* Warn ) {return NULL;}

	// UFactory functions.
	static UObject* StaticImportObject( UClass* InClass, UObject* InOuter, FName InName, DWORD InFlags, const TCHAR* Filename=TEXT(""), UObject* Context=NULL, UFactory* Factory=NULL, const TCHAR* Parms=NULL, FFeedbackContext* Warn=GWarn );
};

// Import an object using a UFactory.
template< class T > T* ImportObject( UObject* Outer, FName Name, DWORD Flags, const TCHAR* Filename=TEXT(""), UObject* Context=NULL, UFactory* Factory=NULL, const TCHAR* Parms=NULL, FFeedbackContext* Warn=GWarn )
{
	return (T*)UFactory::StaticImportObject( T::StaticClass(), Outer, Name, Flags, Filename, Context, Factory, Parms, Warn );
}

/*-----------------------------------------------------------------------------
	UTextBufferFactory.
-----------------------------------------------------------------------------*/

//
// Imports UTextBuffer objects.
//
class CORE_API UTextBufferFactory : public UFactory
{
	DECLARE_CLASS(UTextBufferFactory,UFactory,0)

	// Friends.
	#include "Friends/UTextBufferFactory.h"

	// Constructors.
	UTextBufferFactory();
	void StaticConstructor();

	// UFactory interface.
	UObject* FactoryCreateText( UClass* InClass, UObject* InOuter, FName InName, DWORD InFlags, UObject* Context, const TCHAR* Type, const TCHAR*& Buffer, const TCHAR* BufferEnd, FFeedbackContext* Warn );
};

/*----------------------------------------------------------------------------
	The End.
----------------------------------------------------------------------------*/
