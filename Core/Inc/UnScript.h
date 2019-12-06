/*=============================================================================
	UnScript.h: UnrealScript execution engine.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
	 * Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	Native functions.
-----------------------------------------------------------------------------*/

//
// Native function table.
//
typedef void (UObject::*Native)( FFrame& TheStack, RESULT_DECL );
extern CORE_API Native GNatives[];
BYTE CORE_API GRegisterNative( INT iNative, const Native& Func );

//
// Registering a native function.
//
#if _MSC_VER
	#define IMPLEMENT_FUNCTION(cls,num,func) \
		extern "C" DLL_EXPORT Native int##cls##func = (Native)&cls::func; \
		static BYTE cls##func##Temp = GRegisterNative( num, int##cls##func );
#else
	#define IMPLEMENT_FUNCTION(cls,num,func) \
		extern "C" DLL_EXPORT { Native int##cls##func = (Native)&cls::func; } \
		static BYTE cls##func##Temp = GRegisterNative( num, int##cls##func );
#endif

/*-----------------------------------------------------------------------------
	Macros.
-----------------------------------------------------------------------------*/

//
// Macros for grabbing parameters for native functions.
//
#define P_GET_UBOOL(var)              DWORD var=0;                         Stack.Step( Stack.Object, &var    );
#define P_GET_UBOOL_OPTX(var,def)     DWORD var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_STRUCT(typ,var)         typ   var;                           Stack.Step( Stack.Object, &var    );
#define P_GET_STRUCT_OPTX(typ,var,def)typ   var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_STRUCT_REF(typ,var)     typ   var##T; GPropAddr=0;           Stack.Step( Stack.Object, &var##T ); typ*     var = GPropAddr ? (typ    *)GPropAddr:&var##T;
#define P_GET_INT(var)                INT   var=0;                         Stack.Step( Stack.Object, &var    );
#define P_GET_INT_OPTX(var,def)       INT   var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_INT_REF(var)            INT   var##T=0; GPropAddr=0;         Stack.Step( Stack.Object, &var##T ); INT*     var = GPropAddr ? (INT    *)GPropAddr:&var##T;
#define P_GET_FLOAT(var)              FLOAT var=0.f;                       Stack.Step( Stack.Object, &var    );
#define P_GET_FLOAT_OPTX(var,def)     FLOAT var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_FLOAT_REF(var)          FLOAT var##T=0.f; GPropAddr=0;       Stack.Step( Stack.Object, &var##T ); FLOAT*   var = GPropAddr ? (FLOAT  *)GPropAddr:&var##T;
#define P_GET_BYTE(var)               BYTE  var=0;                         Stack.Step( Stack.Object, &var    );
#define P_GET_BYTE_OPTX(var,def)      BYTE  var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_BYTE_REF(var)           BYTE  var##T=0; GPropAddr=0;         Stack.Step( Stack.Object, &var##T ); BYTE*    var = GPropAddr ? (BYTE   *)GPropAddr:&var##T;
#define P_GET_NAME(var)               FName var=NAME_None;                 Stack.Step( Stack.Object, &var    );
#define P_GET_NAME_OPTX(var,def)      FName var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_NAME_REF(var)           FName var##T=NAME_None; GPropAddr=0; Stack.Step( Stack.Object, &var##T ); FName*   var = GPropAddr ? (FName  *)GPropAddr:&var##T;
#define P_GET_STR(var)                FString var;                         Stack.Step( Stack.Object, &var    );
#define P_GET_STR_OPTX(var,def)       FString var(def);                    Stack.Step( Stack.Object, &var    );
#define P_GET_STR_REF(var)            FString var##T; GPropAddr=0;         Stack.Step( Stack.Object, &var##T ); FString* var = GPropAddr ? (FString*)GPropAddr:&var##T;
#define P_GET_FARRAY(var)             FArray var;                          Stack.Step( Stack.Object, &var    );
#define P_GET_FARRAY_OPTX(var,def)    FArray var(def);                     Stack.Step( Stack.Object, &var    );
#define P_GET_FARRAY_REF(var)         FArray var##T; GPropAddr=0;          Stack.Step( Stack.Object, &var##T ); FArray* var = GPropAddr ? (FArray*)GPropAddr:&var##T;
#define P_GET_TARRAY(typ,var)         TArray<typ> var;                     Stack.Step( Stack.Object, &var    );
#define P_GET_TARRAY_OPTX(typ,var,def)TArray<typ> var(def);                Stack.Step( Stack.Object, &var    );
#define P_GET_TARRAY_REF(typ,var)     TArray<typ> var##T; GPropAddr=0;     Stack.Step( Stack.Object, &var##T ); TArray<typ>* var = GPropAddr ? (TArray<typ>*)GPropAddr:&var##T;
#define P_GET_OBJECT(cls,var)         cls*  var=NULL;                      Stack.Step( Stack.Object, &var    );
#define P_GET_OBJECT_OPTX(cls,var,def)cls*  var=def;                       Stack.Step( Stack.Object, &var    );
#define P_GET_OBJECT_REF(cls,var)     cls*  var##T=NULL; GPropAddr=0;      Stack.Step( Stack.Object, &var##T ); cls**    var = GPropAddr ? (cls   **)GPropAddr:&var##T;
#define P_GET_ARRAY_REF(typ,var)      typ   var##T[256]; GPropAddr=0;      Stack.Step( Stack.Object,  var##T ); typ*     var = GPropAddr ? (typ    *)GPropAddr: var##T;
#define P_GET_SKIP_OFFSET(var)        _WORD var; {checkSlow(*Stack.Code==EX_Skip); Stack.Code++; var=*(_WORD*)Stack.Code; Stack.Code+=2; }
#define P_FINISH                      Stack.Code++;

//
// Convenience macros.
//
#define P_GET_VECTOR(var)           P_GET_STRUCT(FVector,var)
#define P_GET_VECTOR_OPTX(var,def)  P_GET_STRUCT_OPTX(FVector,var,def)
#define P_GET_VECTOR_REF(var)       P_GET_STRUCT_REF(FVector,var)
#define P_GET_ROTATOR(var)          P_GET_STRUCT(FRotator,var)
#define P_GET_ROTATOR_OPTX(var,def) P_GET_STRUCT_OPTX(FRotator,var,def)
#define P_GET_ROTATOR_REF(var)      P_GET_STRUCT_REF(FRotator,var)
#define P_GET_COLOR(var)            P_GET_STRUCT(FColor,var)
#define P_GET_COLOR_OPTX(var,def)   P_GET_STRUCT_OPTX(FColor,var,def)
#define P_GET_COLOR_REF(var)        P_GET_STRUCT_REF(FColor,var)
#define P_GET_COORDS(var)           P_GET_STRUCT(FCoords,var)
#define P_GET_COORDS_OPTX(var,def)  P_GET_STRUCT_OPTX(FCoords,var,def)
#define P_GET_COORDS_REF(var)       P_GET_STRUCT_REF(FCoords,var)
#define P_GET_SCALE(var)            P_GET_STRUCT(FScale,var)
#define P_GET_SCALE_OPTX(var,def)   P_GET_STRUCT_OPTX(FScale,var,def)
#define P_GET_SCALE_REF(var)        P_GET_STRUCT_REF(FScale,var)
#define P_GET_BOX(var)              P_GET_STRUCT(FBox,var)
#define P_GET_BOX_OPTX(var,def)     P_GET_STRUCT_OPTX(FBox,var,def)
#define P_GET_BOX_REF(var)          P_GET_STRUCT_REF(FBox,var)
#define P_GET_SPHERE(var)           P_GET_STRUCT(FSphere,var)
#define P_GET_SPHERE_OPTX(var,def)  P_GET_STRUCT_OPTX(FSphere,var,def)
#define P_GET_SPHERE_REF(var)       P_GET_STRUCT_REF(FSphere,var)
#define P_GET_ACTOR(var)            P_GET_OBJECT(AActor,var)
#define P_GET_ACTOR_OPTX(var,def)   P_GET_OBJECT_OPTX(AActor,var,def)
#define P_GET_ACTOR_REF(var)        P_GET_OBJECT_REF(AActor,var)
#define P_GET_TEXTURE(var)          P_GET_OBJECT(UTexture,var)
#define P_GET_TEXTURE_OPTX(var,def) P_GET_OBJECT_OPTX(UTexture,var,def)
#define P_GET_TEXTURE_REF(var)      P_GET_OBJECT_REF(UTexture,var)
#define P_GET_SOUND(var)            P_GET_OBJECT(USound,var)
#define P_GET_SOUND_OPTX(var,def)   P_GET_OBJECT_OPTX(USound,var,def)
#define P_GET_SOUND_REF(var)        P_GET_OBJECT_REF(USound,var)
#define P_GET_CLASS(var)            P_GET_OBJECT(UClass,var)
#define P_GET_CLASS_OPTX(var,def)   P_GET_OBJECT_OPTX(UClass,var,def)
#define P_GET_CLASS_REF(var)        P_GET_OBJECT_REF(UClass,var)
#define P_GET_DWORD(var)            DWORD var=0;                 Stack.Step( Stack.Object, &var    );
#define P_GET_DWORD_OPTX(var,def)   DWORD var=def;               Stack.Step( Stack.Object, &var    );
#define P_GET_DWORD_REF(var)        DWORD var##T=0; GPropAddr=0; Stack.Step( Stack.Object, &var##T ); DWORD* var = GPropAddr ? (DWORD*)GPropAddr:&var##T;

//
// Bounces a native function to another function.
//
#define DECLARE_EXEC_WRAP( SourceClass, TargetClass ,ExecFunction ) \
	void SourceClass::ExecFunction( FFrame& Stack, RESULT_DECL ) \
	{ \
		((TargetClass*)this)->ExecFunction( Stack, Result ); \
	}

//
// Iterator macros.
//
#define PRE_ITERATOR \
	INT wEndOffset = Stack.ReadWord(); \
	BYTE B=0, Buffer[MAX_CONST_SIZE]; \
	BYTE *StartCode = Stack.Code; \
	do {
#define POST_ITERATOR \
		while( (B=*Stack.Code)!=EX_IteratorPop && B!=EX_IteratorNext ) \
			Stack.Step( Stack.Object, Buffer ); \
		if( *Stack.Code++==EX_IteratorNext ) \
			Stack.Code = StartCode; \
	} while( B != EX_IteratorPop );

/*-----------------------------------------------------------------------------
	FFrame implementation.
-----------------------------------------------------------------------------*/

inline FFrame::FFrame( UObject* InObject )
:	Node		(InObject ? InObject->GetClass() : NULL)
,	Object		(InObject)
,	Code		(NULL)
,	Locals		(NULL)
{}
inline FFrame::FFrame( UObject* InObject, UStruct* InNode, INT CodeOffset, void* InLocals )
:	Node		(InNode)
,	Object		(InObject)
,	Code		(&InNode->Script(CodeOffset))
,	Locals		((BYTE*)InLocals)
{}
inline void FFrame::Step( UObject* Context, RESULT_DECL )
{
	guardSlow(FFrame::Step);
	INT B = *Code++;
	(Context->*GNatives[B])( *this, Result );
	unguardfSlow(( TEXT("(%s @ %s : %04X)"), Object->GetFullName(), Node->GetFullName(), Code - &Node->Script(0) ));
}
inline INT FFrame::ReadInt()
{
	INT Result = *(INT*)Code;
	Code += sizeof(INT);
	return Result;
}
inline UObject* FFrame::ReadObject()
{
	UObject* Result = *(UObject**)Code;
	Code += sizeof(INT);
	return Result;
}
inline FLOAT FFrame::ReadFloat()
{
	FLOAT Result = *(FLOAT*)Code;
	Code += sizeof(FLOAT);
	return Result;
}
inline INT FFrame::ReadWord()
{
	INT Result = *(_WORD*)Code;
	Code += sizeof(_WORD);
	return Result;
}
inline FName FFrame::ReadName()
{
	FName Result = *(FName*)Code;
	Code += sizeof(FName);
	return Result;
}
CORE_API void GInitRunaway();

/*-----------------------------------------------------------------------------
	FStateFrame implementation.
-----------------------------------------------------------------------------*/

inline FStateFrame::FStateFrame( UObject* InObject )
:	FFrame		( InObject )
,	CurrentFrame( NULL )
,	StateNode	( InObject->GetClass() )
,	ProbeMask	( ~(QWORD)0 )
{}
inline const TCHAR* FStateFrame::Describe()
{
	return Node ? Node->GetFullName() : TEXT("None");
}

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
