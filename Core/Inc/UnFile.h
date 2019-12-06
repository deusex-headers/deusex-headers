/*=============================================================================
	UnFile.h: General-purpose file utilities.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.
=============================================================================*/

/*-----------------------------------------------------------------------------
	Global variables.
-----------------------------------------------------------------------------*/

// Global variables.
CORE_API extern DWORD GCRCTable[];

/*----------------------------------------------------------------------------
	Byte order conversion.
----------------------------------------------------------------------------*/

// Bitfields.
#ifndef NEXT_BITFIELD
	#if __INTEL_BYTE_ORDER__
		#define NEXT_BITFIELD(b) ((b)<<1)
		#define FIRST_BITFIELD   (1)
		#define INTEL_ORDER(x)   (x)
	#else
		#define NEXT_BITFIELD(b) ((b)>>1)
		#define FIRST_BITFIELD   (0x80000000)
		#define INTEL_ORDER(x)   (((x)>>24) + (((x)>>8)&0xff00) + (((x)<<8)&0xff0000) + ((x)<<24))
	#endif
#endif

/*-----------------------------------------------------------------------------
	Stats.
-----------------------------------------------------------------------------*/

#if STATS
	#define STAT(x) x
#else
	#define STAT(x)
#endif

/*-----------------------------------------------------------------------------
	Macros.
-----------------------------------------------------------------------------*/

#define DECLARE_INITED(typ,var) typ var; appMemzero(&var,sizeof(var)); var.dwSize=sizeof(var);

/*-----------------------------------------------------------------------------
	Global init and exit.
-----------------------------------------------------------------------------*/

CORE_API void appInit( const TCHAR* InPackage, const TCHAR* InCmdLine, FMalloc* InMalloc, FOutputDevice* InLog, FOutputDeviceError* InError, FFeedbackContext* InWarn, FFileManager* InFileManager, FConfigCache*(*ConfigFactory)(), UBOOL RequireConfig );
CORE_API void appPreExit();
CORE_API void appExit();

/*-----------------------------------------------------------------------------
	Logging and critical errors.
-----------------------------------------------------------------------------*/

CORE_API void appRequestExit( UBOOL Force );

CORE_API void VARARGS appFailAssert( const ANSICHAR* Expr, const ANSICHAR* File, INT Line );
CORE_API void VARARGS appUnwindf( const TCHAR* Fmt, ... );
CORE_API const TCHAR* appGetSystemErrorMessage( INT Error=0 );
CORE_API const void appDebugMessagef( const TCHAR* Fmt, ... );

#define debugf        GLog->Logf
#define warnf         GWarn->Logf
#define warn          GWarn->Log
#define appErrorf     GError->Logf

#if DO_GUARD_SLOW
	#define debugfSlow    GLog->Logf
	#define warnfSlow     GWarn->Logf
	#define appErrorfSlow GError->Logf
#else
	#define debugfSlow    GNull->Logf
	#define warnfSlow     GNull->Logf
	#define appErrorfSlow GNull->Logf
#endif

#if _UNICODE
	#define appPrintf wprintf
#else
	#define appPrintf printf
#endif

/*-----------------------------------------------------------------------------
	Misc.
-----------------------------------------------------------------------------*/

CORE_API void* appGetDllHandle( const TCHAR* DllName );
CORE_API void appFreeDllHandle( void* DllHandle );
CORE_API void* appGetDllExport( void* DllHandle, const TCHAR* ExportName );
CORE_API void appLaunchURL( const TCHAR* URL, const TCHAR* Parms=NULL, FString* Error=NULL );
CORE_API void appCreateProc( const TCHAR* URL, const TCHAR* Parms );
CORE_API void appEnableFastMath( UBOOL Enable ); // CoreI will provide own stub version.
CORE_API class FGuid appCreateGuid();
CORE_API void appCreateTempFilename( const TCHAR* Path, TCHAR* Result256 );
CORE_API void appCleanFileCache();
CORE_API UBOOL appFindPackageFile( const TCHAR* In, const FGuid* Guid, TCHAR* Out );

/*-----------------------------------------------------------------------------
	Clipboard.
-----------------------------------------------------------------------------*/

CORE_API void appClipboardCopy( const TCHAR* Str );
CORE_API FString appClipboardPaste();

/*-----------------------------------------------------------------------------
	Guard macros for call stack display.
-----------------------------------------------------------------------------*/

//
// guard/unguardf/unguard macros.
// For showing calling stack when errors occur in major functions.
// Meant to be enabled in release builds.
//
#if defined(_DEBUG) || !DO_GUARD
	#define guard(func)			{static const TCHAR __FUNC_NAME__[]=TEXT(#func);
	#define unguard				}
	#define unguardf(msg)		}
#else
	#if _MSC_VER
		#define guard(func)			{static const TCHAR __FUNC_NAME__[]=TEXT(#func); try{
		#define unguard				}catch(TCHAR*Err){throw Err;}catch(...){appUnwindf(TEXT("%s"),__FUNC_NAME__); throw;}}
		#define unguardf(msg)		}catch(TCHAR*Err){throw Err;}catch(...){appUnwindf(TEXT("%s"),__FUNC_NAME__); appUnwindf msg; throw;}}
	#else
		#define guard(func)			{static const TCHAR __FUNC_NAME__[]=TEXT(#func); \
									__Context __LOCAL_CONTEXT__; try{ \
									if(setjmp(__Context::Env)) { throw 1; } else {
		#define unguard				}}catch(char*Err){throw Err;}catch(...) \
									{appUnwindf(TEXT("%s"),__FUNC_NAME__); throw;}}
		#define unguardf(msg)		}}catch(char*Err){throw Err;}catch(...) \
									{appUnwindf(TEXT("%s"),__FUNC_NAME__); \
									appUnwindf msg; throw;}}
	#endif
#endif

//
// guardSlow/unguardfSlow/unguardSlow macros.
// For showing calling stack when errors occur in performance-critical functions.
// Meant to be disabled in release builds.
//
#if defined(_DEBUG) || !DO_GUARD || !DO_GUARD_SLOW
	#define guardSlow(func)		{
	#define unguardfSlow(msg)	}
	#define unguardSlow			}
	#define unguardfSlow(msg)	}
#else
	#define guardSlow(func)		guard(func)
	#define unguardSlow			unguard
	#define unguardfSlow(msg)	unguardf(msg)
#endif

//
// For throwing string-exceptions which safely propagate through guard/unguard.
//
CORE_API void VARARGS appThrowf( const TCHAR* Fmt, ... ); // CoreI will provide fixed version.

/*-----------------------------------------------------------------------------
	Check macros for assertions.
-----------------------------------------------------------------------------*/

//
// "check" expressions are only evaluated if enabled.
// "verify" expressions are always evaluated, but only cause an error if enabled.
//
#if DO_CHECK
	#define check(expr)  {if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__ );}
	#define verify(expr) {if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__ );}
#else
	#define check(expr)  
	#define verify(expr) expr
#endif

//
// Check for development only.
//
#if DO_GUARD_SLOW
	#define checkSlow(expr)  {if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__ );}
	#define verifySlow(expr) {if(!(expr)) appFailAssert( #expr, __FILE__, __LINE__ );}
#else
	#define checkSlow(expr)
	#define verifySlow(expr) if(expr){}
#endif

/*-----------------------------------------------------------------------------
	Timing macros.
-----------------------------------------------------------------------------*/

//
// Normal timing.
//
inline void appClock( INT& Timer )
{
	Timer -= appCycles();
}
inline void appUnclock( INT& Timer )
{
	Timer += appCycles();
}
inline void appClockSerialized( INT& Timer )
{
	Timer -= appSerializedCycles();
}
inline void appUnclockSerialized( INT& Timer )
{
	Timer += appSerializedCycles();
}

//
// Performance critical timing.
//
#if DO_CLOCK_SLOW
	inline void appClockSlow            ( INT& Timer ) { appClockSlow            ( Timer ); }
	inline void appUnclockSlow          ( INT& Timer ) { appUnclockSlow          ( Timer ); }
	inline void appClockSerializedSlow  ( INT& Timer ) { appClockSerializedSlow  ( Timer ); }
	inline void appUnclockSerializedSlow( INT& Timer ) { appUnclockSerializedSlow( Timer ); }
#else
	inline void appClockSlow            ( INT& Timer ) {}
	inline void appUnclockSlow          ( INT& Timer ) {}
	inline void appClockSerializedSlow  ( INT& Timer ) {}
	inline void appUnclockSerializedSlow( INT& Timer ) {}
#endif

/*-----------------------------------------------------------------------------
	Text format.
-----------------------------------------------------------------------------*/

CORE_API FString appFormat( FString Src, const TMultiMap<FString,FString>& Map );

/*-----------------------------------------------------------------------------
	Localization.
-----------------------------------------------------------------------------*/

// DEUS_EX CNN
#if DEUS_EX
CORE_API UBOOL appCheckGermanSystem(void); // CoreI will provide own a version always returning false.
#endif

CORE_API const TCHAR* Localize( const TCHAR* Section, const TCHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL, UBOOL Optional=0 );
CORE_API const TCHAR* LocalizeError( const TCHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL );
CORE_API const TCHAR* LocalizeProgress( const TCHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL );
CORE_API const TCHAR* LocalizeQuery( const TCHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL );
CORE_API const TCHAR* LocalizeGeneral( const TCHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL );

#ifdef _UNICODE
	CORE_API const TCHAR* Localize( const ANSICHAR* Section, const ANSICHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL, UBOOL Optional=0 );
	CORE_API const TCHAR* LocalizeError( const ANSICHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL );
	CORE_API const TCHAR* LocalizeProgress( const ANSICHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL );
	CORE_API const TCHAR* LocalizeQuery( const ANSICHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL );
	CORE_API const TCHAR* LocalizeGeneral( const ANSICHAR* Key, const TCHAR* Package=GPackage, const TCHAR* LangExt=NULL );
#endif

/*-----------------------------------------------------------------------------
	File utilities.
-----------------------------------------------------------------------------*/

CORE_API const TCHAR* appFExt( const TCHAR* Filename );
CORE_API UBOOL appUpdateFileModTime( TCHAR* Filename ); // !! Add to FFileManagerI
CORE_API FString appGetGMTRef();

/*-----------------------------------------------------------------------------
	OS functions.
-----------------------------------------------------------------------------*/

CORE_API const TCHAR* appCmdLine();
CORE_API const TCHAR* appBaseDir();
CORE_API const TCHAR* appPackage();
CORE_API const TCHAR* appComputerName();
CORE_API const TCHAR* appUserName();

/*-----------------------------------------------------------------------------
	Timing functions.
-----------------------------------------------------------------------------*/

#if !DEFINED_appCycles
CORE_API DWORD appCycles();
#endif

#if !DEFINED_appSeconds
CORE_API DOUBLE appSeconds();
#endif

CORE_API void appSystemTime( INT& Year, INT& Month, INT& DayOfWeek, INT& Day, INT& Hour, INT& Min, INT& Sec, INT& MSec );
CORE_API const TCHAR* appTimestamp();
CORE_API DOUBLE appSecondsSlow();
CORE_API void appSleep( FLOAT Seconds );

/*-----------------------------------------------------------------------------
	Character type functions.
-----------------------------------------------------------------------------*/

inline TCHAR appToUpper( INT Ch )
{
	return (TCHAR)((Ch<'a' || Ch>'z') ? Ch : (Ch+'A'-'a'));
}
inline TCHAR appToLower( INT Ch )
{
	return (TCHAR)((Ch<'A' || Ch>'Z') ? Ch : (Ch+'a'-'A'));
}
inline UBOOL appIsAlpha( INT Ch )
{
	return (Ch>='a' && Ch<='z') || (Ch>='A' && Ch<='Z');
}
inline UBOOL appIsDigit( INT Ch )
{
	return Ch>='0' && Ch<='9';
}
inline UBOOL appIsAlnum( INT Ch )
{
	return appIsAlpha(Ch) || appIsDigit(Ch);
}
inline UBOOL appIsHexDigit( INT Ch )
{
	if ( appIsDigit(Ch) )
		return 1;
	Ch = appToUpper(Ch);
	return (Ch>='A' && Ch<='F');
}
inline INT appHexDigitToDec( INT Ch )
{
	if ( appIsDigit(Ch) )
		return Ch-'0';
	Ch = appToUpper(Ch);
	if ( Ch>='A' && Ch<='F' )
		return Ch-'A'+10;
	return 0; // Failed.
}
inline ANSICHAR appAnsiToUpper( ANSICHAR c )
{
	return (c<'a' || c>'z') ? (c) : (ANSICHAR)(c+'A'-'a');
}
inline ANSICHAR appAnsiToLower( ANSICHAR c )
{
	return (c<'A' || c>'Z') ? (c) : (ANSICHAR)(c+'a'-'A');
}
inline UBOOL appAnsiIsAlpha( ANSICHAR c )
{
	return (c>='a' && c<='z') || (c>='A' && c<='Z');
}
inline UBOOL appAnsiIsDigit( ANSICHAR c )
{
	return c>='0' && c<='9';
}
inline UBOOL appAnsiIsAlnum( ANSICHAR c )
{
	return (c>='a' && c<='z') || (c>='A' && c<='Z') || (c>='0' && c<='9');
}

/*-----------------------------------------------------------------------------
	String functions.
-----------------------------------------------------------------------------*/

// Comparison.
CORE_API INT appStrcmp( const TCHAR* A, const TCHAR* B );
CORE_API INT appStricmp( const TCHAR* A, const TCHAR* B );
CORE_API INT appStrncmp( const TCHAR* A, const TCHAR* B, INT Count );
CORE_API INT appStrnicmp( const TCHAR* A, const TCHAR* B, INT Count );

// Copy.
CORE_API TCHAR* appStrcpy( TCHAR* Dest, const TCHAR* Src );
CORE_API TCHAR* appStrncpy( TCHAR* Dest, const TCHAR* Src, INT Max );
CORE_API INT appStrcpy( const TCHAR* Str );

// Length.
CORE_API INT appStrlen( const TCHAR* Str );

// Concatenation.
CORE_API TCHAR* appStrcat( TCHAR* Dest, const TCHAR* Src );
CORE_API TCHAR* appStrncat( TCHAR* Dest, const TCHAR* Src, INT Max );

// Search.
CORE_API TCHAR* appStrstr( const TCHAR* Str, const TCHAR* Find );
CORE_API TCHAR* appStrchr( const TCHAR* Str, INT c );

// Static strings. Use appDynamicString instead.
#if defined(_REALLY_WANT_STATIC_STRING)
	CORE_API TCHAR* appStaticString1024();
	CORE_API ANSICHAR* appAnsiStaticString1024();
#endif

// Formating.
CORE_API INT appSprintf( TCHAR* Dest, const TCHAR* Fmt, ... ); // CoreI will provide fixed version.
CORE_API INT appGetVarArgs( TCHAR* Dest, INT Count, const TCHAR*& Fmt ); // CoreI will provide fixed version.

// Upper/Lower.
CORE_API TCHAR* appStrupr( TCHAR* Str );

// Atoi/Atof.
CORE_API INT appAtoi( const TCHAR* Str );
CORE_API FLOAT appAtof( const TCHAR* Str );

// Case insensitive string hash functions.
inline DWORD appStrihash( const TCHAR* Data )
{
	guardSlow(appStrihash);
	DWORD Hash=0;
	while( *Data )
	{
		TCHAR Ch = appToUpper(*Data++);
		BYTE  B  = (BYTE)Ch;
		Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
#if UNICODE
		B        = (BYTE)(Ch>>8);
		Hash     = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
#endif
	}
	return Hash;
	unguardSlow;
}
inline DWORD appAnsiStrihash( const ANSICHAR* Data )
{
	guardSlow(appAnsiStrihash);
	DWORD Hash=0;
	while( *Data )
	{
		ANSICHAR Ch = appAnsiToUpper(*Data++);
		BYTE     B  = (BYTE)Ch;
		Hash        = ((Hash >> 8) & 0x00FFFFFF) ^ GCRCTable[(Hash ^ B) & 0x000000FF];
	}
	return Hash;
	unguardSlow;
}

// Misc.
CORE_API const TCHAR* appStrfind(const TCHAR* Str, const TCHAR* Find);
CORE_API const TCHAR* appSpc( INT Num ); // CoreI will provide fixed version.
CORE_API DWORD appStrCrc( const TCHAR* Data );
CORE_API DWORD appStrCrcCaps( const TCHAR* Data );
CORE_API INT appStrtoi( const TCHAR* Start, TCHAR** End, INT Base );

// To/From.
CORE_API const ANSICHAR* appToAnsi( const TCHAR* Str );
CORE_API const UNICHAR* appToUnicode( const TCHAR* Str );
CORE_API const TCHAR* appFromAnsi( const ANSICHAR* Str );
CORE_API const TCHAR* appFromUnicode( const UNICHAR* Str );
CORE_API UBOOL appIsPureAnsi( const TCHAR* Str );

/*-----------------------------------------------------------------------------
	Parsing functions.
-----------------------------------------------------------------------------*/

CORE_API UBOOL ParseCommand( const TCHAR** Stream, const TCHAR* Match );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, class FName& Name );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, DWORD& Value );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, class FGuid& Guid );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, TCHAR* Value, INT MaxLen );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, BYTE& Value );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, SBYTE& Value );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, _WORD& Value );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, SWORD& Value );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, FLOAT& Value );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, INT& Value );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, FString& Value );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, QWORD& Value );
CORE_API UBOOL Parse( const TCHAR* Stream, const TCHAR* Match, SQWORD& Value );
CORE_API UBOOL ParseUBOOL( const TCHAR* Stream, const TCHAR* Match, UBOOL& OnOff );
CORE_API UBOOL ParseObject( const TCHAR* Stream, const TCHAR* Match, class UClass* Type, class UObject*& DestRes, class UObject* InParent );
CORE_API UBOOL ParseLine( const TCHAR** Stream, TCHAR* Result, INT MaxLen, UBOOL Exact=0 );
CORE_API UBOOL ParseLine( const TCHAR** Stream, FString& Resultd, UBOOL Exact=0 );
CORE_API UBOOL ParseToken( const TCHAR*& Str, TCHAR* Result, INT MaxLen, UBOOL UseEscape );
CORE_API UBOOL ParseToken( const TCHAR*& Str, FString& Arg, UBOOL UseEscape );
CORE_API FString ParseToken( const TCHAR*& Str, UBOOL UseEscape );
CORE_API void ParseNext( const TCHAR** Stream );
CORE_API UBOOL ParseParam( const TCHAR* Stream, const TCHAR* Param );

/*-----------------------------------------------------------------------------
	QSort.
-----------------------------------------------------------------------------*/

typedef INT QSORT_RETURN;
typedef QSORT_RETURN(CDECL* QSORT_COMPARE)( const void* A, const void* B );
CORE_API void appQsort( void* Base, INT Num, INT Width, QSORT_COMPARE Compare );

/*-----------------------------------------------------------------------------
	Math functions.
-----------------------------------------------------------------------------*/

// So newer compilers in theory could do a better job.
#if defined(_REALLY_WANT_CORE_TRIGONOMETRY_AND_FRIENDS) || 1
	CORE_API DOUBLE appExp( DOUBLE Value );
	CORE_API DOUBLE appLoge( DOUBLE Value );
	CORE_API DOUBLE appFmod( DOUBLE A, DOUBLE B );
	CORE_API DOUBLE appSin( DOUBLE Value );
	CORE_API DOUBLE appCos( DOUBLE Value );
	CORE_API DOUBLE appTan( DOUBLE Value );
	CORE_API DOUBLE appAtan( DOUBLE Value );
	CORE_API DOUBLE appAtan2( DOUBLE Y, DOUBLE X );
	CORE_API DOUBLE appSqrt( DOUBLE Value );
	CORE_API DOUBLE appPow( DOUBLE A, DOUBLE B );
#else
	#define appExp(x)     exp(x)
	#define appLoge(x)    log(x)
	#define appFmod(a,b)  fmod(a,b)
	#define appSin(x)     sin(x)
	#define appCos(x)     cos(x)
	#define appTan(x)     tan(x)
	#define appAtan(x)    atan(x)
	#define appAtan2(y,x) atan2(y,x)
	#define appSqrt(x)    sqrt(x)
	#define appPow(a,b)   pow(a,b)
#endif

// appRand only returns numbers up to APP_RAND_MAX.
#define APP_RAND_MAX 0x7FFF
CORE_API INT appRand();
CORE_API FLOAT appFrand();

#ifndef DEFINED_appIsNan
CORE_API UBOOL appIsNan( DOUBLE Value );
#endif

#if !DEFINED_appRound
CORE_API INT appRound( FLOAT Value );
#endif

#if !DEFINED_appFloor
CORE_API INT appFloor( FLOAT Value );
#endif

#if !DEFINED_appCeil
CORE_API INT appCeil( FLOAT Value );
#endif

// 0 is not considered positive by this function.
inline UBOOL appIsPowerOfTwo( INT i, UBOOL Positive=1 )
{
	guardSlow(appIsPowerOfTwo);
	if ( i<=0 )
	{
		if ( Positive )
			return 0;
		
		// We don't do imaginary integer here.
		return 0;
	}
	return !(i&(i-1));
	unguardSlow;
}

/*-----------------------------------------------------------------------------
	Math functions implemented inside CoreI.
-----------------------------------------------------------------------------*/

#if !DEFINED_appIsFinite
COREI_API appIsFinite( DOUBLE Value );
#endif

#if defined(_REALLY_WANT_CORE_TRIGONOMETRY_AND_FRIENDS) || 1
	COREI_API DOUBLE appAcos( DOUBLE Value );
	COREI_API DOUBLE appAsin( DOUBLE Value );
#else
	#define appAcos(x) acos(x)
	#define appAsin(x) asin(x)
#endif

/*------------------------------------------------------------------------------------
	Approximate math functions.
------------------------------------------------------------------------------------*/

//
// Famous Fast inverse square root.
//
// A bit on the history of it:
// https://www.beyond3d.com/content/articles/8/ (tldr author unknown)
//
// A bit more detailed view, constant derivation and error bounds are
// discussed here:
// https://www.lomont.org/Math/Papers/2003/InvSqrt.pdf
// 
// Nomenclature: Nn, where n is the number of newton iterations.
//
// max rel. err.
//   n=0: (unknown) Large.
//   n=1: 0.175124  Rather large for resonable work, but should be enough for rejection work.
//   n=2: 0.000465  Quite good.
//   n=3: (unknown) Should be enough for almost everything.
//
// So use n=2 as a about safe default.
#define appFastInvSqrt(x) appFastInvSqrtN2(x)
//

inline FLOAT appFastInvSqrtN0( FLOAT X )
{
	INT I = *(INT*)&X;

	// Trick.
	I = 0x5f375a86-(I>>1);
	X = *(FLOAT*)&I;

	// No Newton iteration.
	return X;
}
inline FLOAT appFastInvSqrtN1( FLOAT X )
{
	FLOAT H = 0.5f*X; INT I = *(INT*)&X;

	// Trick.
	I = 0x5f375a86-(I>>1);
	X = *(FLOAT*)&I;

	// Single Newton iteration.
	X = X*(1.5f-H*X*X);
	return X;
}
inline FLOAT appFastInvSqrtN2( FLOAT X )
{
	FLOAT H = 0.5f*X; INT I = *(INT*)&X;

	// Trick.
	I = 0x5f375a86-(I>>1);
	X = *(FLOAT*)&I;

	// Two Newton iterations.
	X = X*(1.5f-H*X*X);
	X = X*(1.5f-H*X*X);
	return X;
}
inline FLOAT appFastInvSqrtN3( FLOAT X )
{
	FLOAT H = 0.5f*X; INT I = *(INT*)&X;

	// Trick.
	I = 0x5f375a86-(I>>1);
	X = *(FLOAT*)&I;

	// Three Newton iterations.
	X = X*(1.5f-H*X*X);
	X = X*(1.5f-H*X*X);
	X = X*(1.5f-H*X*X);
	return X;
}

// Conveniance.
inline FLOAT appFastSqrt  ( FLOAT X ) { return 1.f/appFastInvSqrt  (X); }
inline FLOAT appFastSqrtN0( FLOAT X ) { return 1.f/appFastInvSqrtN0(X); }
inline FLOAT appFastSqrtN1( FLOAT X ) { return 1.f/appFastInvSqrtN1(X); }
inline FLOAT appFastSqrtN2( FLOAT X ) { return 1.f/appFastInvSqrtN2(X); }
inline FLOAT appFastSqrtN3( FLOAT X ) { return 1.f/appFastInvSqrtN3(X); }

//
// Taylor series expansion of arccos.
//
#define ATP_PI (3.1415926535897932f)
inline FLOAT appAcosTaylorP1( FLOAT X )
{
	return (ATP_PI/2.f)-X;
}
inline FLOAT appAcosTaylorP3( FLOAT X )
{
	//return (PI/2)-X-(1.f/6.f)*X*X*X;
	return (ATP_PI/2.f)-X*(1.f+(1.f/6.f)*X*X);
}
inline FLOAT appAcosTaylorP5( FLOAT X )
{
	//return (PI/2)-X-(1.f/6.f)*X*X*X-(3.f/40.f)*X*X*X*X*X;
	return (ATP_PI/2.f)-X*(1.f+X*X*((1.f/6.f)+X*X*(3.f/40.f)));
}
inline FLOAT appAcosTaylorP7( FLOAT X )
{
	//return (PI/2)-X-(1.f/6.f)*X*X*X-(3.f/40.f)*X*X*X*X*X-(5.f/112.f)*X*X*X*X*X*X*X;
	return (ATP_PI/2.f)-X*(1.f+X*X*((1.f/6.f)+X*X*((3.f/40.f)+X*X*(5.f/112.f))));
}
inline FLOAT appAcosTaylorP9( FLOAT X )
{
	return (ATP_PI/2.f)-X-(1.f/6.f)*X*X*X-(3.f/40.f)*X*X*X*X*X-(5.f/112.f)*X*X*X*X*X*X*X-(35.f/1152.f)*X*X*X*X*X*X*X*X*X;
}

/*-----------------------------------------------------------------------------
	Array functions.
-----------------------------------------------------------------------------*/

// Core functions depending on TArray and FString.
CORE_API UBOOL appLoadFileToArray( TArray<BYTE>& Result, const TCHAR* Filename, FFileManager* FileManager=GFileManager );
CORE_API UBOOL appLoadFileToString( FString& Result, const TCHAR* Filename, FFileManager* FileManager=GFileManager );
CORE_API UBOOL appSaveArrayToFile( const TArray<BYTE>& Array, const TCHAR* Filename, FFileManager* FileManager=GFileManager );
CORE_API UBOOL appSaveStringToFile( const FString& String, const TCHAR* Filename, FFileManager* FileManager=GFileManager );

/*-----------------------------------------------------------------------------
	Memory functions.
-----------------------------------------------------------------------------*/

CORE_API void* appMemmove( void* Dest, const void* Src, INT Count );
CORE_API INT appMemcmp( const void* Buf1, const void* Buf2, INT Count );
CORE_API UBOOL appMemIsZero( const void* V, int Count );
CORE_API DWORD appMemCrc( const void* Data, INT Length, DWORD CRC=0 );
CORE_API void appMemswap( void* Ptr1, void* Ptr2, DWORD Size );
CORE_API void appMemset( void* Dest, INT C, INT Count );

#ifndef DEFINED_appMemcpy
CORE_API void appMemcpy( void* Dest, const void* Src, INT Count );
#endif

#ifndef DEFINED_appMemzero
CORE_API void appMemzero( void* Dest, INT Count );
#endif

//
// C style memory allocation stubs.
//
#define appMalloc     GMalloc->Malloc
#define appFree       GMalloc->Free
#define appRealloc    GMalloc->Realloc

//
// C++ style memory allocation.
//
inline void* operator new( unsigned int Size, const TCHAR* Tag )
{
	return appMalloc( Size, Tag );
}
inline void* operator new( unsigned int Size )
{
	return appMalloc( Size, TEXT("new") );
}
inline void operator delete( void* Ptr )
{
	appFree( Ptr );
}

#if PLATFORM_NEEDS_ARRAY_NEW
inline void* operator new[]( unsigned int Size, const TCHAR* Tag )
{
	return appMalloc( Size, Tag );
}
inline void* operator new[]( unsigned int Size )
{
	return appMalloc( Size, TEXT("new") );
}
inline void operator delete[]( void* Ptr )
{
	appFree( Ptr );
}
#endif

/*-----------------------------------------------------------------------------
	Math.
-----------------------------------------------------------------------------*/

CORE_API BYTE appCeilLogTwo( DWORD Arg );

/*-----------------------------------------------------------------------------
	MD5 functions.
-----------------------------------------------------------------------------*/

//
// MD5 Context.
//
struct FMD5Context
{
	DWORD state[4];
	DWORD count[2];
	BYTE buffer[64];
};

//
// MD5 functions.
//!!it would be cool if these were implemented as subclasses of
// FArchive.
//
CORE_API void appMD5Init( FMD5Context* context );
CORE_API void appMD5Update( FMD5Context* context, BYTE* input, INT inputLen );
CORE_API void appMD5Final( BYTE* digest, FMD5Context* context );
CORE_API void appMD5Transform( DWORD* state, BYTE* block );
CORE_API void appMD5Encode( BYTE* output, DWORD* input, INT len );
CORE_API void appMD5Decode( DWORD* output, BYTE* input, INT len );

/*-----------------------------------------------------------------------------
	Misc.
-----------------------------------------------------------------------------*/

// Make a "four character code" DWORD from a four-character string.
static DWORD appFourCC( const TCHAR* Ch )
{
	if( !Ch )
		return 0;
	INT Len=appStrlen(Ch);
	return
		((DWORD)(BYTE)(Len>0?Ch[0]:32) << 0 )
	|	((DWORD)(BYTE)(Len>1?Ch[1]:32) << 8 )
	|	((DWORD)(BYTE)(Len>2?Ch[2]:32) << 16)
	|	((DWORD)(BYTE)(Len>3?Ch[3]:32) << 24);
}

// Make a "eight character code" QWORD from a eight-character string.
static QWORD appEightCC( const TCHAR* Ch )
{
	if( !Ch )
		return 0;
	INT Len=appStrlen(Ch);
	return
		((QWORD)(BYTE)(Len>0?Ch[0]:32) << 0 )
	|	((QWORD)(BYTE)(Len>1?Ch[1]:32) << 8 )
	|	((QWORD)(BYTE)(Len>2?Ch[2]:32) << 16)
	|	((QWORD)(BYTE)(Len>3?Ch[3]:32) << 24)
	|	((QWORD)(BYTE)(Len>0?Ch[0]:32) << 32)
	|	((QWORD)(BYTE)(Len>1?Ch[1]:32) << 40)
	|	((QWORD)(BYTE)(Len>2?Ch[2]:32) << 48)
	|	((QWORD)(BYTE)(Len>3?Ch[3]:32) << 56);
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
