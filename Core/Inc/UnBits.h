/*=============================================================================
	UnBits.h: Unreal bitstream manipulation classes.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	FBitWriter.
-----------------------------------------------------------------------------*/

//
// Writes bitstreams.
//
struct CORE_API FBitWriter : public FArchive
{
	friend struct FBitWriterMark;
public:
	FBitWriter( INT InMaxBits );
	void SerializeBits( void* Src, INT LengthBits );
	void SerializeInt( DWORD& Value, DWORD Max );
	void WriteInt( DWORD Result, DWORD Max );
	void WriteBit( BYTE In );
	void Serialize( void* Src, INT LengthBytes );
	BYTE* GetData();
	INT GetNumBytes();
	INT GetNumBits();
	void SetOverflowed();

private:
	TArray<BYTE> Buffer;
	INT   Num;
	INT   Max;
};

//
// For pushing and popping FBitWriter positions.
//
struct CORE_API FBitWriterMark
{
public:
	FBitWriterMark()
	:	Num         ( 0 )
	{}
	FBitWriterMark( FBitWriter& Writer )
	:	Overflowed	( Writer.ArIsError )
	,	Num			( Writer.Num )
	{}
	INT GetNumBits()
	{
		return Num;
	}
	void Pop( FBitWriter& Writer );
private:
	UBOOL			Overflowed;
	INT				Num;
};

/*-----------------------------------------------------------------------------
	FBitReader.
-----------------------------------------------------------------------------*/

//
// Reads bitstreams.
//
struct FBitReader : public FArchive
{
public:
	CORE_API FBitReader( BYTE* Src=NULL, INT CountBits=0 );
	CORE_API void SetData( FBitReader& Src, INT CountBits );
	CORE_API void SerializeBits( void* Dest, INT LengthBits );
	CORE_API void SerializeInt( DWORD& Value, DWORD Max );
	CORE_API DWORD ReadInt( DWORD Max );
	CORE_API BYTE ReadBit();
	DWORD ReadBits( INT NumBits )
	{
		guardSlow(FBitReader::ReadBits);
		checkSlow(NumBits>=0);
		checkSlow(NumBits<=32);

		DWORD Result=0;
		for ( INT Bit=0; Bit<NumBits; Bit++ )
			Result |= DWORD(ReadBit())<<Bit;

		return Result;
		unguardSlow;
	}
	CORE_API void Serialize( void* Dest, INT LengthBytes );
	CORE_API BYTE* GetData();
	CORE_API UBOOL AtEnd();
	CORE_API void SetOverflowed();
	CORE_API INT GetNumBytes();
	CORE_API INT GetNumBits();
	CORE_API INT GetPosBits();
private:
	TArray<BYTE> Buffer;
	INT   Num;
	INT   Pos;
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
