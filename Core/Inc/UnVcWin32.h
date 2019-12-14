/*=============================================================================
	UnVcWin32.h: Unreal definitions for Visual C++ SP2 running under Win32.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

#define _WIN32_WINNT 0x0600

/*-----------------------------------------------------------------------------
	Includes.
-----------------------------------------------------------------------------*/

// These are needed to define extended functionality inside the headers.
#include <cmath>

/*----------------------------------------------------------------------------
	Platform compiler definitions.
----------------------------------------------------------------------------*/

#define __WIN32__				1
#define __INTEL__				1
#define __INTEL_BYTE_ORDER__	1

/*----------------------------------------------------------------------------
	Platform specifics types and defines.
----------------------------------------------------------------------------*/

// Undo any Windows defines.
#undef BYTE
#undef WORD
#undef DWORD
#undef INT
#undef UINT
#undef FLOAT
#undef MAXBYTE
#undef MAXWORD
#undef MAXDWORD
#undef MAXINT
#undef CDECL
#undef UNREFERENCED_PARAMETER

// Make sure HANDLE is defined.
#ifndef _WINDOWS_
	#define HANDLE void*
	#define HINSTANCE void*
#endif

// Sizes.
#define DEFAULT_ALIGNMENT       16   // Default boundary to align memory allocations on.
#define CACHE_LINE_SIZE         64   // Cache line size.
#define HALF_CACHE_LINE_SIZE    32   // Align macro won't do math in VC6.
#define QUARTER_CACHE_LINE_SIZE 16   // Align macro won't do math in VC6.
#define PAGE_SIZE               4096 // Page size.

// Optimization macros (preceeded by #pragma).
#define DISABLE_OPTIMIZATION optimize("",off)
#define ENABLE_OPTIMIZATION  optimize("",on)

// Function type macros.
#define DLL_IMPORT	__declspec(dllimport)	/* Import function from DLL */
#define DLL_EXPORT  __declspec(dllexport)	/* Export function to DLL */
#define DLL_EXPORT_CLASS	__declspec(dllexport)	/* Export class to DLL */
#define VARARGS     __cdecl					/* Functions with variable arguments */
#define CDECL	    __cdecl					/* Standard C function */
#define STDCALL		__stdcall				/* Standard calling convention */
#define FORCEINLINE __forceinline			/* Force code to be inline */
#define ZEROARRAY                           /* Zero-length arrays in structs */

// Variable arguments.
#define GET_VARARGS(msg,len,fmt) appGetVarArgs(msg,len,fmt)

// Compiler name.
#ifdef _DEBUG
	#define COMPILER "Compiled with Visual C++ Debug"
#else
	#define COMPILER "Compiled with Visual C++"
#endif

// Unsigned base types.
typedef unsigned char		BYTE;		// 8-bit  unsigned.
typedef unsigned short		_WORD;		// 16-bit unsigned.
typedef unsigned long		DWORD;		// 32-bit unsigned.
typedef unsigned int		UINT;		// 32-bit unsigned.
typedef unsigned __int64	QWORD;		// 64-bit unsigned.

// Signed base types.
typedef	signed char			SBYTE;		// 8-bit  signed.
typedef signed short		SWORD;		// 16-bit signed.
typedef signed int  		INT;		// 32-bit signed.
typedef signed __int64		SQWORD;		// 64-bit signed.

// Character types.
typedef char				ANSICHAR;	// An ANSI character.
typedef unsigned short      UNICHAR;	// A unicode character.
typedef unsigned char		ANSICHARU;	// An ANSI character.
typedef unsigned short      UNICHARU;	// A unicode character.

// Other base types.
typedef signed int			UBOOL;		// Boolean 0 (false) or 1 (true).
typedef float				FLOAT;		// 32-bit IEEE floating point.
typedef double				DOUBLE;		// 64-bit IEEE double.
typedef unsigned long       SIZE_T;     // Corresponds to C SIZE_T.
typedef unsigned int        UINT; // 32-bit unsigned integer, but with int instead of long type compared to DWORD.
typedef signed int          PTRDIFF_T;

// Bitfield type.
typedef unsigned long       BITFIELD;	// For bitfields.

// Unwanted VC++ level 4 warnings to disable.
#pragma warning(disable : 4244) /* conversion to float, possible loss of data							*/
#pragma warning(disable : 4699) /* creating precompiled header											*/
#pragma warning(disable : 4200) /* Zero-length array item at end of structure, a VC-specific extension	*/
#pragma warning(disable : 4100) /* unreferenced formal parameter										*/
#pragma warning(disable : 4514) /* unreferenced inline function has been removed						*/
#pragma warning(disable : 4201) /* nonstandard extension used : nameless struct/union					*/
#pragma warning(disable : 4710) /* inline function not expanded											*/
#pragma warning(disable : 4702) /* unreachable code in inline expanded function							*/
#pragma warning(disable : 4711) /* function selected for autmatic inlining								*/
#pragma warning(disable : 4725) /* Pentium fdiv bug														*/
#pragma warning(disable : 4127) /* Conditional expression is constant									*/
#pragma warning(disable : 4512) /* assignment operator could not be generated                           */
#pragma warning(disable : 4530) /* C++ exception handler used, but unwind semantics are not enabled     */
#pragma warning(disable : 4245) /* conversion from 'enum ' to 'unsigned long', signed/unsigned mismatch */
#pragma warning(disable : 4305) /* truncation from 'const double' to 'float'                            */
#pragma warning(disable : 4238) /* nonstandard extension used : class rvalue used as lvalue             */
#pragma warning(disable : 4251) /* needs to have dll-interface to be used by clients of class 'ULinker' */
#pragma warning(disable : 4275) /* non dll-interface class used as base for dll-interface class         */
#pragma warning(disable : 4511) /* copy constructor could not be generated                              */
#pragma warning(disable : 4284) /* return type is not a UDT or reference to a UDT                       */
#pragma warning(disable : 4355) /* this used in base initializer list                                   */
#pragma warning(disable : 4097) /* typedef-name '' used as synonym for class-name ''                    */
#pragma warning(disable : 4291) /* typedef-name '' used as synonym for class-name ''                    */
#pragma warning(disable : 4505) /* Unreferenced local function has been removed                         */

// If C++ exception handling is disabled, force guarding to be off.
#ifndef _CPPUNWIND
	#error "Bad VCC option: C++ exception handling must be enabled"
#endif

// Make sure characters are unsigned.
#ifdef _CHAR_UNSIGNED
	#error "Bad VC++ option: Characters must be signed"
#endif

// No asm if not compiling for x86.
#ifndef _M_IX86
	#undef ASM
	#define ASM 0
#endif

// If no asm, redefine __asm to cause compile-time error.
#if !ASM
	#define __asm ERROR_ASM_NOT_ALLOWED
#endif

// Strings.
#define LINE_TERMINATOR TEXT("\r\n")
#define PATH_SEPARATOR TEXT("\\")

// DLL file extension.
#define DLLEXT TEXT(".dll")

// Pathnames.
#define PATH(s) s

// NULL.
#define NULL 0

// It helps... sometimes.
#ifndef UNREFERENCED_PARAMETER
	#define UNREFERENCED_PARAMETER(P) (P)
#endif

// Package implementation.
#define IMPLEMENT_PACKAGE_PLATFORM(pkgname) \
	extern "C" {HINSTANCE hInstance;} \
	INT DLL_EXPORT STDCALL DllMain( HINSTANCE hInInstance, DWORD Reason, void* Reserved ) \
	{ hInstance = hInInstance; return 1; }

// Platform support options.
#define PLATFORM_NEEDS_ARRAY_NEW 1
#define FORCE_ANSI_LOG           1

// OS unicode function calling.
#if defined(NO_UNICODE_OS_SUPPORT) || !defined(UNICODE)
	#define TCHAR_CALL_OS(funcW,funcA) (funcA)
	#define TCHAR_TO_ANSI(str) str
	#define ANSI_TO_TCHAR(str) str
#elif defined(NO_ANSI_OS_SUPPORT)
	#define TCHAR_CALL_OS(funcW,funcA) (funcW)
	#define TCHAR_TO_ANSI(str) str
	#define ANSI_TO_TCHAR(str) str
#else
	CORE_API ANSICHAR* winToANSI( ANSICHAR* ACh, const UNICHAR* InUCh, INT Count );
	CORE_API INT winGetSizeANSI( const UNICHAR* InUCh );
	CORE_API UNICHAR* winToUNICODE( UNICHAR* Ch, const ANSICHAR* InUCh, INT Count );
	CORE_API INT winGetSizeUNICODE( const ANSICHAR* InACh );
	#define TCHAR_CALL_OS(funcW,funcA) (GUnicodeOS ? (funcW) : (funcA))
	#define TCHAR_TO_ANSI(str) winToANSI((ANSICHAR*)appAlloca(winGetSizeANSI(str)),str,winGetSizeANSI(str))
	#define TCHAR_TO_OEM(str) winToOEM((ANSICHAR*)appAlloca(winGetSizeANSI(str)),str,winGetSizeANSI(str))
	#define ANSI_TO_TCHAR(str) winToUNICODE((TCHAR*)appAlloca(winGetSizeUNICODE(str)*sizeof(TCHAR)),str,winGetSizeUNICODE(str))
#endif

// GCC Workarounds.
#define GCC_OPT_INLINE inline
#define GCC_PACK(n)
#define GCC_ALIGN(n)

// Alignment
#define ALIGN_STRUCT(n) __declspec(align(n))

// Win32 stuff.
#define DECLARE_INITED(typ,var) typ var; appMemzero(&var,sizeof(var)); var.dwSize=sizeof(var);

/*----------------------------------------------------------------------------
	Globals.
----------------------------------------------------------------------------*/

// System identification.
extern "C"
{
	extern HINSTANCE      hInstance;
	extern CORE_API UBOOL GIsMMX;
	extern CORE_API UBOOL GIsPentiumPro;
	extern CORE_API UBOOL GIsKatmai;
	extern CORE_API UBOOL GIsK6;
	extern CORE_API UBOOL GIs3DNow;
	extern CORE_API UBOOL GTimestamp;
}

// Katmai New Instructions (KNI) aka SSE.
#define GIsSSE GIsKatmai

// System identification (CoreI).
extern "C"
{
	extern COREI_API UBOOL GIsTscInvariant;
	extern COREI_API UBOOL GIsSSE2;
	extern COREI_API UBOOL GIsSSE3;
	extern COREI_API UBOOL GIsAVX;
}

/*----------------------------------------------------------------------------
	Functions.
----------------------------------------------------------------------------*/

//
// Round a floating point number to an integer.
// Note that (int+.5) is rounded to (int+1).
//
#if ASM
#define DEFINED_appRound 1
inline INT appRound( FLOAT F )
{
	INT I;
	__asm fld [F]
	__asm fistp [I]
	return I;
}
#endif

//
// Converts to integer equal to or less than.
//
#if ASM
#define DEFINED_appFloor 1
inline INT appFloor( FLOAT F )
{
	static FLOAT Half=0.5;
	INT I;
	__asm fld [F]
	__asm fsub [Half]
	__asm fistp [I]
	return I;
}
#endif

//
// CPU cycles, related to GSecondsPerCycle.
//
#if ASM
#define DEFINED_appCycles 1
#pragma warning (push)
#pragma warning (disable : 4035)
#pragma warning (disable : 4715)
inline DWORD appCycles()
{
	DWORD L;

	__asm
	{
		xor edx,edx // Required so that VC++ realizes EDX is modified.
		xor eax,eax // Required so that VC++ realizes EAX is modified.
		_emit 0x0F  // RDTSC  -  Pentium+ time stamp register to EDX:EAX.
		_emit 0x31  // Use only 32 bits in EAX - even a Ghz cpu would have a 4+ sec period.
		mov [L],eax // Save low value.
	}

	return L;
}
#pragma warning (pop)


#if ASM
#define DEFINED_appQuadCycles 1
#pragma warning (push)
#pragma warning (disable : 4035)
#pragma warning (disable : 4715)
inline QWORD appQuadCycles()
{
	union
	{
		struct
		{
			QWORD R;
		};
		struct
		{
			DWORD L;
			DWORD H;
		};
	} S;
	__asm
	{
		xor eax,eax   // Required so that VC++ realizes EAX is modified.
		xor edx,edx   // Required so that VC++ realizes EDX is modified.
		_emit 0x0F    // RDTSC  -  Pentium+ time stamp register to EDX:EAX.
		_emit 0x31    // 
		mov [S.L],eax // Save low value.
		mov [S.H],edx // Save high value.
	}
	return S.R;
}
#pragma warning (pop)
#endif



#define DEFINED_appSerializedCycles 1
#pragma warning (push)
#pragma warning (disable : 4035)
#pragma warning (disable : 4715)
inline DWORD appSerializedCycles()
{
	DWORD L;

	__asm
	{
		xor edx,edx // Required so that VC++ realizes EDX is modified.
		xor eax,eax // Required so that VC++ realizes EAX is modified.
		_emit 0x0F  // RDTSC  -  Pentium+ time stamp register to EDX:EAX.
		_emit 0x01
		_emit 0xF9
		mov [L],eax // Save low value.
	}

	return L;
}
#pragma warning (pop)
#endif
#if ASM
#define DEFINED_appSerializedQuadCycles 1
#pragma warning (push)
#pragma warning (disable : 4035)
#pragma warning (disable : 4715)
inline QWORD appSerializedQuadCycles()
{
	union
	{
		struct
		{
			QWORD R;
		};
		struct
		{
			DWORD L;
			DWORD H;
		};
	} S;
	__asm
	{
		xor eax,eax   // Required so that VC++ realizes EAX is modified.
		xor edx,edx   // Required so that VC++ realizes EDX is modified.
		_emit 0x0F    // RDTSC  -  Pentium+ time stamp register to EDX:EAX.
		_emit 0x01    // 
		mov [S.L],eax // Save low value.
		mov [S.H],edx // Save high value.
	}
	return S.R;
}
#pragma warning (pop)
#endif

//
// Seconds, arbitrarily based.
//
#if ASM
#define DEFINED_appSeconds 1
#pragma warning (push)
#pragma warning (disable : 4035)
extern CORE_API DOUBLE GSecondsPerCycle;
CORE_API DOUBLE appSecondsSlow();
inline DOUBLE appSeconds()
{
	if( GTimestamp )
	{
		DWORD L,H;
		__asm
		{
			xor eax,eax	// Required so that VC++ realizes EAX is modified.
			xor edx,edx	// Required so that VC++ realizes EDX is modified.
			_emit 0x0F  // RDTSC  -  Pentium+ time stamp register to EDX:EAX.
			_emit 0x31  //
			mov [L],eax // Save low value.
			mov [H],edx // Save high value.
		}
		return ((DOUBLE)L +  4294967296.0 * (DOUBLE)H) * GSecondsPerCycle;
	}
	else return appSecondsSlow();
}
#pragma warning (pop)
#endif

//
// Memory copy.
//
#if ASM
#define DEFINED_appMemcpy
inline void appMemcpy( void* Dest, const void* Src, INT Count )
{	
	__asm
	{
		mov		ecx, Count
		mov		esi, Src
		mov		edi, Dest
		mov     ebx, ecx
		shr     ecx, 2
		and     ebx, 3
		rep     movsd
		mov     ecx, ebx
		rep     movsb
	}
}
#endif

//
// Memory zero.
//
#if ASM
#define DEFINED_appMemzero
inline void appMemzero( void* Dest, INT Count )
{	
	__asm
	{
		mov		ecx, [Count]
		mov		edi, [Dest]
		xor     eax, eax
		mov		ebx, ecx
		shr		ecx, 2
		and		ebx, 3
		rep     stosd
		mov     ecx, ebx
		rep     stosb
	}
}
#endif

#if ASM
#define DEFINED_appDebugBreak
inline void appDebugBreak()
{
	__asm
	{
		int 3
	}
}
#endif

extern "C" void* __cdecl _alloca(size_t);
#define appAlloca(size) _alloca((size+DEFAULT_ALIGNMENT-1)&~(DEFAULT_ALIGNMENT-1))

/*-----------------------------------------------------------------------------
	Make sure proper FourCC macro is defined..
-----------------------------------------------------------------------------*/

#ifdef FCC
	#undef FCC
#endif

#define FCC(ch4) ((((DWORD)(ch4) & 0xFF) << 24) |     \
                  (((DWORD)(ch4) & 0xFF00) << 8) |    \
                  (((DWORD)(ch4) & 0xFF0000) >> 8) |  \
                  (((DWORD)(ch4) & 0xFF000000) >> 24))

/*----------------------------------------------------------------------------
	The End.
----------------------------------------------------------------------------*/
