/*=============================================================================
	UnTemplate.h: Unreal templates.
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	Type information.
-----------------------------------------------------------------------------*/

//
// Type information for initialization.
//
template <class T> struct TTypeInfoBase
{
public:
	typedef const T& ConstInitType;
	static UBOOL NeedsDestructor() {return 1;}
	static UBOOL DefinitelyNeedsDestructor() {return 0;}
	static const T& ToInit( const T& In ) {return In;}
};
template <class T> struct TTypeInfo : public TTypeInfoBase<T>
{
};

template <> struct TTypeInfo<BYTE> : public TTypeInfoBase<BYTE>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<SBYTE> : public TTypeInfoBase<SBYTE>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<ANSICHAR> : public TTypeInfoBase<ANSICHAR>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<INT> : public TTypeInfoBase<INT>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<DWORD> : public TTypeInfoBase<DWORD>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<_WORD> : public TTypeInfoBase<_WORD>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<SWORD> : public TTypeInfoBase<SWORD>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<QWORD> : public TTypeInfoBase<QWORD>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<SQWORD> : public TTypeInfoBase<SQWORD>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<FName> : public TTypeInfoBase<FName>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};
template <> struct TTypeInfo<UObject*> : public TTypeInfoBase<UObject*>
{
public:
	static UBOOL NeedsDestructor() {return 0;}
};

/*-----------------------------------------------------------------------------
	Standard templates.
-----------------------------------------------------------------------------*/

template< class T > inline T Abs( const T A )
{
	return (A>=(T)0) ? A : -A;
}
template< class T > inline T Sgn( const T A )
{
	return (A>0) ? 1 : ((A<0) ? -1 : 0);
}
template< class T > inline T Max( const T A, const T B )
{
	return (A>=B) ? A : B;
}
template< class T > inline T Max( const T A, const T B, const T C )
{
	T X = Max<T>( A, B );
	return (X>=C) ? X : C;
}
template< class T > inline T Max( const T A, const T B, const T C, const T D )
{
	T X = Max<T>( A, B );
	T Y = Max<T>( C, D );
	return (X>=Y) ? X : Y;
}
template< class T > inline T Min( const T A, const T B )
{
	return (A<=B) ? A : B;
}
template< class T > inline T Min( const T A, const T B, const T C )
{
	T X = Min<T>( A, B );
	return (X<=C) ? X : C;
}
template< class T > inline T Min( const T A, const T B, const T C, const T D )
{
	T X = Min<T>( A, B );
	T Y = Min<T>( C, D );
	return (X<=Y) ? X : Y;
}
template< class T > inline T Square( const T A )
{
	return A*A;
}
template< class T > inline T Cube( const T A )
{
	return A*A*A;
}
template< class T > inline T Quad( const T A )
{
	return A*A*A*A;
}
template< class T > inline T Clamp( const T X, const T Min, const T Max )
{
	return X<Min ? Min : X<Max ? X : Max;
}
template< class T > inline T Align( const T Ptr, INT Alignment )
{
	return (T)(((DWORD)Ptr + Alignment - 1) & ~(Alignment-1));
}
template< class T > inline void Exchange( T& A, T& B )
{
	const T Temp = A;
	A = B;
	B = Temp;
}
template< class T > T Lerp( T& A, T& B, FLOAT Alpha )
{
	return A + Alpha * (B-A);
}
inline DWORD GetTypeHash( const BYTE A )
{
	return A;
}
inline DWORD GetTypeHash( const SBYTE A )
{
	return A;
}
inline DWORD GetTypeHash( const _WORD A )
{
	return A;
}
inline DWORD GetTypeHash( const SWORD A )
{
	return A;
}
inline DWORD GetTypeHash( const INT A )
{
	return A;
}
inline DWORD GetTypeHash( const DWORD A )
{
	return A;
}
inline DWORD GetTypeHash( const QWORD A )
{
	return (DWORD)A+((DWORD)(A>>32) * 23);
}
inline DWORD GetTypeHash( const SQWORD A )
{
	return (DWORD)A+((DWORD)(A>>32) * 23);
}
inline DWORD GetTypeHash( const TCHAR* S )
{
	return appStrihash(S);
}
#define ExchangeB(A,B) { UBOOL T=A; A=B; B=T; }

/*----------------------------------------------------------------------------
	Standard macros.
----------------------------------------------------------------------------*/

// Number of elements in an array.
#define ARRAY_COUNT(array) \
	( sizeof(array)/sizeof((array)[0]) )

#define STRUCT_OFFSET(struc,member) \
	( (SIZE_T)&((struc*)NULL)->member )

/*-----------------------------------------------------------------------------
	Allocators.
-----------------------------------------------------------------------------*/

template <class T> class TAllocator
{};

/*-----------------------------------------------------------------------------
	Dynamic array template.
-----------------------------------------------------------------------------*/

//
// Base dynamic array.
//
class FArray
{
public:
	void* GetData()
	{
		return Data;
	}
	const void* GetData() const
	{
		return Data;
	}
	UBOOL IsValidIndex( INT i ) const
	{
		return i>=0 && i<ArrayNum;
	}
	INT Num() const
	{
		guardSlow(FArray::Num);
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);

		return ArrayNum;
		unguardSlow;
	}
	void InsertZeroed( INT Index, INT Count, INT ElementSize )
	{
		guardSlow(FArray::InsertZeroed);
		checkSlow(ElementSize>0);
		checkSlow(Index>=0);
		checkSlow(Index<=ArrayNum);
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);

		Insert( Index, Count, ElementSize );
		appMemzero( (BYTE*)Data+Index*ElementSize, Count*ElementSize );
		unguardSlow;
	}
	void Insert( INT Index, INT Count, INT ElementSize )
	{
		guardSlow(FArray::Insert);
		checkSlow(Count>=0);
		checkSlow(ElementSize>0);
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);
		checkSlow(Index>=0);
		checkSlow(Index<=ArrayNum);

		INT OldNum = ArrayNum;
		if( (ArrayNum+=Count)>ArrayMax )
		{
			ArrayMax = ArrayNum + 3*ArrayNum/8 + 32;
			Realloc( ElementSize );
		}
		appMemmove
		(
			(BYTE*)Data + (Index+Count )*ElementSize,
			(BYTE*)Data + (Index       )*ElementSize,
			              (OldNum-Index)*ElementSize
		);

		unguardSlow;
	}
	INT Add( INT Count, INT ElementSize )
	{
		guardSlow(FArray::Add);
		checkSlow(Count>=0);
		checkSlow(ElementSize>0);
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);

		INT Index = ArrayNum;
		if( (ArrayNum+=Count)>ArrayMax )
		{
			ArrayMax = ArrayNum + 3*ArrayNum/8 + 32;
			Realloc( ElementSize );
		}

		return Index;
		unguardSlow;
	}
	INT AddZeroed( INT ElementSize, INT n=1 )
	{
		guardSlow(FArray::AddZeroed);
		checkSlow(ElementSize>0);
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);

		INT Index = Add( n, ElementSize );
		appMemzero( (BYTE*)Data+Index*ElementSize, n*ElementSize );
		return Index;
		unguardSlow;
	}
	void Shrink( INT ElementSize )
	{
		guardSlow(FArray::Shrink);
		checkSlow(ElementSize>0);
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);

		if( ArrayMax != ArrayNum )
		{
			ArrayMax = ArrayNum;
			Realloc( ElementSize );
		}
		unguardSlow;
	}
	void SetSize( INT Count, INT ElementSize )
	{
		guardSlow(FArray::SetSize);
		checkSlow(Count>=0);
		checkSlow(ElementSize>0);
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);
		ArrayNum = ArrayMax = Count;
		Realloc( ElementSize );
		unguardSlow;
	}
	void Empty( INT ElementSize, INT Slack=0 )
	{
		guardSlow(FArray::Empty);
		checkSlow(ElementSize>0);
		checkSlow(Slack>=0);
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);
		ArrayNum = 0;
		ArrayMax = Slack;
		Realloc( ElementSize );
		unguardSlow;
	}
	FArray()
	:	Data	( NULL )
	,	ArrayNum( 0 )
	,	ArrayMax( 0 )
	{}
	FArray( ENoInit )
	{}
	~FArray()
	{
		if( Data )
			appFree( Data );
		Data = NULL;
		ArrayNum = ArrayMax = 0;
	}
	void CountBytes( FArchive& Ar, INT ElementSize )
	{
		guardSlow(FArray::CountBytes);
		Ar.CountBytes( ArrayNum*ElementSize, ArrayMax*ElementSize );
		unguardSlow;
	}
	CORE_API void Remove( INT Index, INT Count, INT ElementSize );
protected:
	CORE_API void Realloc( INT ElementSize ); // !! CoreI will provide own version.
	FArray( INT InNum, INT ElementSize )
	:	Data    ( NULL  )
	,	ArrayNum( InNum )
	,	ArrayMax( InNum )
	{
		guardSlow(FArray::FArray);
		Realloc( ElementSize );
		unguardSlow;
	}
	void* Data;
	INT	  ArrayNum;
	INT	  ArrayMax;
};

//
// Templated dynamic array.
//
template< class T > class TArray : public FArray
{
public:
	typedef T ElementType;
	TArray()
	:	FArray()
	{}
	TArray( INT InNum )
	:	FArray( InNum, sizeof(T) )
	{}
	TArray( const TArray& Other )
	:	FArray( Other.ArrayNum, sizeof(T) )
	{
		guardSlow(TArray::copyctor);
		if( TTypeInfo<T>::NeedsDestructor() )
		{
			ArrayNum=0;
			for( INT i=0; i<Other.ArrayNum; i++ )
				new(*this)T(Other(i));
		}
		else if( sizeof(T)!=1 )
		{
			for( INT i=0; i<ArrayNum; i++ )
				(*this)(i) = Other(i);
		}
		else if ( ArrayNum>0 )
		{
			guardSlow(DoCopy);
			appMemcpy( &(*this)(0), &Other(0), ArrayNum * sizeof(T) );
			unguardSlow;
		}
		unguardSlow;
	}
	TArray( ENoInit )
	: FArray( E_NoInit )
	{}
	~TArray()
	{
		checkSlow(ArrayNum>=0);
		checkSlow(ArrayMax>=ArrayNum);
		Remove( 0, ArrayNum );
	}
	T& operator()( INT i )
	{
		guardSlow(TArray::operator());
		checkSlow(i>=0);
		checkSlow(i<=ArrayNum);
		checkSlow(ArrayMax>=ArrayNum);
		checkSlow(ArrayMax>0);
		checkSlow(Data);
		return ((T*)Data)[i];
		unguardSlow;
	}
	const T& operator()( INT i ) const
	{
		guardSlow(TArray::operator());
		checkSlow(i>=0);
		checkSlow(i<=ArrayNum);
		checkSlow(ArrayMax>=ArrayNum);
		checkSlow(ArrayMax>0);
		checkSlow(Data);
		return ((T*)Data)[i];
		unguardSlow;
	}
	T Pop()
	{
		guardSlow(TArray::Pop);
		check(ArrayNum>0);
		checkSlow(ArrayMax>=ArrayNum);
		T Result = ((T*)Data)[ArrayNum-1];
		Remove( ArrayNum-1 );
		return Result;
		unguardSlow;
	}
	T& Last( INT c=0 )
	{
		guardSlow(TArray::Last);
		check(c<ArrayNum);
		checkSlow(ArrayMax>=ArrayNum);
		return ((T*)Data)[ArrayNum-c-1];
		unguardSlow;
	}
	const T& Last( INT c=0 ) const
	{
		guardSlow(TArray::Last);
		checkSlow(c<ArrayNum);
		checkSlow(ArrayMax>=ArrayNum);
		return ((T*)Data)[ArrayNum-c-1];
		unguardSlow;
	}
	void Shrink()
	{
		guardSlow(TArray::Shrink);
		FArray::Shrink( sizeof(T) );
		unguardSlow;
	}
	UBOOL FindItem( const T& Item, INT& Index ) const
	{
		guardSlow(TArray::FindItem);
		for( Index=0; Index<ArrayNum; Index++ )
			if( (*this)(Index)==Item )
				return 1;
		return 0;
		unguardSlow;
	}
	INT FindItemIndex( const T& Item ) const
	{
		guardSlow(TArray::FindItemIndex);
		for( INT Index=0; Index<ArrayNum; Index++ )
			if( (*this)(Index)==Item )
				return Index;
		return INDEX_NONE;
		unguardSlow;
	}
	friend FArchive& operator<<( FArchive& Ar, TArray& A )
	{
		guard(TArray<<);
		A.CountBytes( Ar );
		if( sizeof(T)==1 )
		{
			// Serialize simple bytes which require no construction or destruction.
			Ar << AR_INDEX(A.ArrayNum);
			if( Ar.IsLoading() )
			{
				A.ArrayMax = A.ArrayNum;
				A.Realloc( sizeof(T) );
			}
			if ( A.Num() > 0 )
				Ar.Serialize( &A(0), A.Num() );
			//else
				//Ar.Serialize( NULL, 0 );
		}
		else if( Ar.IsLoading() )
		{
			// Load array.
			INT NewNum=0;
			Ar << AR_INDEX(NewNum);
			A.Empty( NewNum );
			for( INT i=0; i<NewNum; i++ )
				Ar << *new(A)T;
		}
		else
		{
			// Save array.
			Ar << AR_INDEX(A.ArrayNum);
			for( INT i=0; i<A.ArrayNum; i++ )
				Ar << A( i );
		}
		return Ar;
		unguard;
	}
	void CountBytes( FArchive& Ar )
	{
		guardSlow(TArray::CountBytes);
		FArray::CountBytes( Ar, sizeof(T) );
		unguardSlow;
	}

	// Add, Insert, Remove, Empty interface.
	INT Add( INT n=1 )
	{
		guardSlow(TArray::Add);
		checkSlow(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		return FArray::Add( n, sizeof(T) );
		unguardSlow;
	}
	void SetSize( INT n=1 )
	{
		guardSlow(TArray::SetSize);
		checkSlow(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		FArray::SetSize( n, sizeof(T) );
		unguardSlow;
	}
	void Insert( INT Index, INT Count=1 )
	{
		guardSlow(TArray::Insert);
		checkSlow(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		FArray::Insert( Index, Count, sizeof(T) );
		unguardSlow;
	}
	void InsertZeroed( INT Index, INT Count=1 )
	{
		guardSlow(TArray::InsertZeroed);
		checkSlow(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		FArray::InsertZeroed( Index, Count, sizeof(T) );
		unguardSlow;
	}
	void Remove( INT Index, INT Count=1 )
	{
		guardSlow(TArray::Remove);
		check(Index>=0);
		check(Index<=ArrayNum);
		check(Index+Count<=ArrayNum);
		if( TTypeInfo<T>::NeedsDestructor() )
			for( INT i=Index; i<Index+Count; i++ )
				(&(*this)(i))->~T();
		FArray::Remove( Index, Count, sizeof(T) );
		unguardSlow;
	}
	void Empty( INT Slack=0 )
	{
		guardSlow(TArray::Empty);
		if( TTypeInfo<T>::NeedsDestructor() )
			for( INT i=0; i<ArrayNum; i++ )
				(&(*this)(i))->~T();
		FArray::Empty( sizeof(T), Slack );
		unguardSlow;
	}

	// Functions dependent on Add, Remove.
	TArray& operator+( const TArray& Other )
	{
		guardSlow(TArray::operator=);
		if( this != &Other )
		{
			for( INT i=0; i<Other.ArrayNum; i++ )
				new( *this )T( Other(i) );
		}
		return *this;
		unguardSlow;
	}
	TArray& operator+=( const TArray& Other )
	{
		guardSlow(TArray::operator=);
		if( this != &Other )
		{
			*this = *this + Other;
		}
		return *this;
		unguardSlow;
	}
	TArray& operator=( const TArray& Other )
	{
		guardSlow(TArray::operator=);
		if( this != &Other )
		{
			Empty( Other.ArrayNum );
			for( INT i=0; i<Other.ArrayNum; i++ )
				new( *this )T( Other(i) );
		}
		return *this;
		unguardSlow;
	}
	INT AddItem( const T& Item )
	{
		guardSlow(TArray::AddItem);
		checkSlow(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		new(*this) T(Item);
		return Num()-1;
		unguardSlow;
	}
	void InsertItem( INT Index, const T& Item )
	{
		guardSlow(TArray::InsertItem);
		Insert( Index );
		(*this)(Index)=Item;
		unguardSlow;
	}
	INT AddZeroed( INT n=1 )
	{
		guardSlow(TArray::AddZeroed);
		return FArray::AddZeroed( sizeof(T), n );
		unguardSlow;
	}
	INT AddUniqueItem( const T& Item )
	{
		guardSlow(TArray::AddUniqueItem);
		checkSlow(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		for( INT Index=0; Index<ArrayNum; Index++ )
			if( (*this)(Index)==Item )
				return Index;
		return AddItem( Item );
		unguardSlow;
	}
	INT RemoveItem( const T& Item )
	{
		guardSlow(TArray::RemoveItem);
		INT OriginalNum=ArrayNum;
		for( INT Index=0; Index<ArrayNum; Index++ )
			if( (*this)(Index)==Item )
				Remove( Index-- );
		return OriginalNum - ArrayNum;
		unguardSlow;
	}

	// Iterator.
	class TIterator
	{
	public:
		TIterator( TArray<T>& InArray ) : Array(InArray), Index(-1) { ++*this;      }
		void operator++()      { ++Index;                                           }
		void RemoveCurrent()   { Array.Remove(Index--); }
		INT GetIndex()   const { return Index;                                      }
		operator UBOOL() const { return Index < Array.Num();                        }
		T& operator*()   const { return Array(Index);                               }
		T* operator->()  const { return &Array(Index);                              }
		T& GetCurrent()  const { return Array( Index );                             }
		T& GetPrev()     const { return Array( Index ? Index-1 : Array.Num()-1 );   }
		T& GetNext()     const { return Array( Index<Array.Num()-1 ? Index+1 : 0 ); }
	private:
		TArray<T>& Array;
		INT Index;
	};
};
template<class T> struct TArrayNoInit : public TArray<T>
{
	TArrayNoInit()
	: TArray<T>( E_NoInit )
	{}
	TArrayNoInit& operator=( const TArrayNoInit& Other )
	{
		TArray<T>::operator=( Other );
		return *this;
	}
};

//
// Array operator news.
//
template <class T> void* operator new( size_t Size, TArray<T>& Array )
{
	INT Index = Array.FArray::Add(1,sizeof(T));
	return &Array(Index);
}
template <class T> void* operator new( size_t Size, TArray<T>& Array, INT Index )
{
	Array.FArray::Insert(Index,1,sizeof(T));
	return &Array(Index);
}

//
// Array exchanger.
//
template <class T> inline void ExchangeArray( TArray<T>& A, TArray<T>& B )
{
	guardSlow(ExchangeTArray);
	appMemswap( &A, &B, sizeof(FArray) );
	unguardSlow;
}

/*-----------------------------------------------------------------------------
	Transactional array.
-----------------------------------------------------------------------------*/

template< class T > class TTransArray : public TArray<T>
{
public:
	// Constructors.
	TTransArray( UObject* InOwner, INT InNum=0 )
	:	TArray<T>( InNum )
	,	Owner( InOwner )
	{
		checkSlow(Owner);
	}
	TTransArray( UObject* InOwner, const TArray<T>& Other )
	:	TArray<T>( Other )
	,	Owner( InOwner )
	{
		checkSlow(Owner);
	}
	TTransArray& operator=( const TTransArray& Other )
	{
		operator=( (const TArray<T>&)Other );
		return *this;
	}

	// Add, Insert, Remove, Empty interface.
	INT Add( INT Count=1 )
	{
		guardSlow(TTransArray::Add);
		INT Index = TArray<T>::Add( Count );
		if ( GUndo )
			GUndo->SaveArray( Owner, this, Index, Count, 1, sizeof(T), SerializeItem, DestructItem );
		return Index;
		unguardSlow;
	}
	void Insert( INT Index, INT Count=1 )
	{
		guardSlow(TTransArray::InsertZeroed);
		FArray::Insert( Index, Count, sizeof(T) );
		if ( GUndo )
			GUndo->SaveArray( Owner, this, Index, Count, 1, sizeof(T), SerializeItem, DestructItem );
		unguardSlow;
	}
	void Remove( INT Index, INT Count=1 )
	{
		guardSlow(TTransArray::Remove);
		if ( GUndo )
			GUndo->SaveArray( Owner, this, Index, Count, -1, sizeof(T), SerializeItem, DestructItem );
		TArray<T>::Remove( Index, Count );
		unguardSlow;
	}
	void Empty( INT Slack=0 )
	{
		guardSlow(TTransArray::Empty);
		if ( GUndo )
			GUndo->SaveArray( Owner, this, 0, ArrayNum, -1, sizeof(T), SerializeItem, DestructItem );
		TArray<T>::Empty( Slack );
		unguardSlow;
	}

	// Functions dependent on Add, Remove.
	TTransArray& operator=( const TArray<T>& Other )
	{
		guardSlow(TTransArray::operator=);
		if( this != &Other )
		{
			Empty( Other.Num() );
			for( INT i=0; i<Other.Num(); i++ )
				new( *this )T( Other(i) );
		}
		return *this;
		unguardSlow;
	}
	INT AddItem( const T& Item )
	{
		guardSlow(TTransArray::AddItem);
		checkSlow(!TTypeInfo<T>::DefinitelyNeedsDestructor());
		new(*this) T(Item);
		return Num()-1;
		unguardSlow;
	}
	INT AddZeroed( INT n=1 )
	{
		guardSlow(TTransArray::AddZeroed);
		INT Index = Add(n);
		appMemzero( &(*this)(Index), n*sizeof(T) );
		return Index;
		unguardSlow;
	}
	INT AddUniqueItem( const T& Item )
	{
		guardSlow(TTransArray::AddUniqueItem);
		for ( INT Index=0; Index<ArrayNum; Index++ )
			if ( (*this)(Index)==Item )
				return Index;
		return AddItem( Item );
		unguardSlow;
	}
	INT RemoveItem( const T& Item )
	{
		guardSlow(TTransArray::RemoveItem);
		INT OriginalNum=ArrayNum;
		for ( INT Index=0; Index<ArrayNum; Index++ )
			if ( (*this)(Index)==Item )
				Remove( Index-- );
		return OriginalNum - ArrayNum;
		unguardSlow;
	}

	// TTransArray interface.
	UObject* GetOwner()
	{
		return Owner;
	}
	void ModifyItem( INT Index )
	{
		guardSlow(TTransArray::ModifyItem);
		if( GUndo )
			GUndo->SaveArray( Owner, this, Index, 1, 0, sizeof(T), SerializeItem, DestructItem );
		unguardSlow;
	}
	void ModifyAllItems()
	{
		guardSlow(TTransArray::ModifyAllItems);
		if( GUndo )
			GUndo->SaveArray( Owner, this, 0, Num(), 0, sizeof(T), SerializeItem, DestructItem );
		unguardSlow;
	}
	friend FArchive& operator<<( FArchive& Ar, TTransArray& A )
	{
		guard(TTransArray<<);
		if( !Ar.IsTrans() )
			Ar << (TArray<T>&)A;
		return Ar;
		unguard;
	}
protected:
	static void SerializeItem( FArchive& Ar, void* TPtr )
	{
		guardSlow(TArray::SerializeItem);
		Ar << *(T*)TPtr;
		unguardSlow;
	}
	static void DestructItem( void* TPtr )
	{
		guardSlow(TArray::SerializeItem);
		((T*)TPtr)->~T();
		unguardSlow;
	}
	UObject* Owner;
private:

	// Disallow the copy constructor.
	TTransArray( const TArray<T>& Other )
	{}
};

//
// Transactional array operator news.
//
template <class T> void* operator new( size_t Size, TTransArray<T>& Array )
{
	guardSlow(TArray::operator new);
	INT Index = Array.Add();
	return &Array(Index);
	unguardSlow;
}
template <class T> void* operator new( size_t Size, TTransArray<T>& Array, INT Index )
{
	guardSlow(TArray::operator new);
	Array.Insert(Index);
	return &Array(Index);
	unguardSlow;
}

/*-----------------------------------------------------------------------------
	Lazy loading.
-----------------------------------------------------------------------------*/

//
// Lazy loader base class.
//
class FLazyLoader
{
	friend class ULinkerLoad;
//protected:
public:
	FArchive*	 SavedAr;
	INT          SavedPos;
public:
	FLazyLoader()
	: SavedAr( NULL )
	, SavedPos( 0 )
	{}
	virtual void Load()=0;
	virtual void Unload()=0;
};

//
// Lazy-loadable dynamic array.
//
template <class T> class TLazyArray : public TArray<T>, public FLazyLoader
{
public:
	TLazyArray( INT InNum=0 )
	: TArray<T>( InNum )
	, FLazyLoader()
	{}
	~TLazyArray()
	{
		if( SavedAr )
			SavedAr->DetachLazyLoader( this );
	}
#if LOAD_ON_DEMAND /* Breaks because of untimely accesses of operator() !! */
	T& operator()( INT i )
	{
		guardSlow(TArray::operator());
		checkSlow(i>=0);
		checkSlow(i<=ArrayNum);
		checkSlow(ArrayMax>=ArrayNum);
		if( SavedPos>0 )
			Load();
		return ((T*)Data)[i];
		unguardSlow;
	}
	const T& operator()( INT i ) const
	{
		guardSlow(TArray::operator());
		checkSlow(i>=0);
		checkSlow(i<=ArrayNum);
		checkSlow(ArrayMax>=ArrayNum);
		if( SavedPos>0 )
			Load();
		return ((T*)Data)[i];
		unguardSlow;
	}
#endif
	void Load()
	{
		// Make sure this array is loaded.
		guard(TLazyArray::Load);
		if( SavedPos>0 )
		{
			// Lazy load it now.
			INT PushedPos = SavedAr->Tell();
			SavedAr->Seek( SavedPos );
			*SavedAr << (TArray<T>&)*this;
			SavedPos *= -1;
			SavedAr->Seek( PushedPos );
		}
		unguard;
	}
	void Unload()
#if __GNUG__
		;	// Function declaration.
#else
	{
		// Make sure this array is unloaded.
		guard(TLazyArray::Unload);
		if( SavedPos<0 )
		{
			// Unload it now.
			Empty();
			SavedPos *= -1;
		}
		unguard;
	}
#endif
	friend FArchive& operator<<( FArchive& Ar, TLazyArray& This )
	{
		guard(TLazyArray<<);
		if( Ar.IsLoading() )
		{
			INT SeekPos=0;
			if( Ar.Ver() <= 61 )
			{
				//oldver: Handles dynamic arrays of fixed-length serialized items only.
				Ar.AttachLazyLoader( &This );
				INT SkipCount=0;
				Ar << AR_INDEX(SkipCount);
				SeekPos = Ar.Tell() + SkipCount*sizeof(T);
					
			}
			else
			{
				Ar << SeekPos;
				Ar.AttachLazyLoader( &This );
			}
			if( !GLazyLoad )
				This.Load();
			Ar.Seek( SeekPos );
		}
		else if( Ar.IsSaving() && Ar.Ver()>61 )
		{
			// Save out count for skipping over this data.
			INT CountPos = Ar.Tell();
			Ar << CountPos << (TArray<T>&)This;
			INT EndPos = Ar.Tell();
			Ar.Seek( CountPos );
			Ar << EndPos;
			Ar.Seek( EndPos );
		}
		else Ar << (TArray<T>&)This;
		return Ar;
		unguard;
	}
};
#if __GNUG__
template <class T> void TLazyArray<T>::Unload()
{
	// Make sure this array is unloaded.
	 guard(TLazyArray::Unload);
	 if( SavedPos<0 && GLazyLoad )
	 {
		// Unload it now.
		Empty();
		SavedPos *= -1;
	 }
	 unguard;
}
#endif

/*-----------------------------------------------------------------------------
	Dynamic strings.
-----------------------------------------------------------------------------*/

//
// A dynamically sizeable string.
//
class FString : protected TArray<TCHAR>
{
public:
	FString()
	: TArray<TCHAR>()
	{}
	FString( const FString& Other )
	: TArray<TCHAR>( Other.ArrayNum )
	{
		if( ArrayNum )
			appMemcpy( &(*this)(0), &Other(0), ArrayNum*sizeof(TCHAR) );
	}
	FString( const TCHAR* In )
	: TArray<TCHAR>( (In && *In) ? (appStrlen(In)+1) : 0 )
	{
		if( ArrayNum )
			appMemcpy( &(*this)(0), In, ArrayNum*sizeof(TCHAR) );
	}
	FString( ENoInit )
	: TArray<TCHAR>( E_NoInit )
	{}
	// What is this?
	explicit CORE_API FString( BYTE   Arg, INT Digits=1 );
	explicit CORE_API FString( SBYTE  Arg, INT Digits=1 );
	explicit CORE_API FString( _WORD  Arg, INT Digits=1 );
	explicit CORE_API FString( SWORD  Arg, INT Digits=1 );
	explicit CORE_API FString( INT    Arg, INT Digits=1 );
	explicit CORE_API FString( DWORD  Arg, INT Digits=1 );
	explicit CORE_API FString( FLOAT  Arg, INT Digits=1, INT RightDigits=0, UBOOL LeadZero=1 );
	explicit CORE_API FString( DOUBLE Arg, INT Digits=1, INT RightDigits=0, UBOOL LeadZero=1 );
	FString& operator=( const TCHAR* Other )
	{
		guardSlow(FString:: operator=(const TCHAR*));
		if( Data==NULL || &(*this)(0)!=Other )
		{
			ArrayNum = ArrayMax = (Other && *Other) ? appStrlen(Other)+1 : 0;
			Realloc( sizeof(TCHAR) );
			if( ArrayNum )
				appMemcpy( &(*this)(0), Other, ArrayNum*sizeof(TCHAR) );
		}
		return *this;
		unguardSlow;
	}
	FString& operator=( const FString& Other )
	{
		guardSlow(FString:: operator=(const FString&));
		if( Data==NULL || this!=&Other )
		{
			checkSlow(Other.Num()>=0);
			//checkSlow(Other.GetData()!=NULL);
			ArrayNum = ArrayMax = Other.Num();
			guardSlow(DoRealloc);
			Realloc( sizeof(TCHAR) );
			unguardSlow;
			if( ArrayNum && &(*this)(0) )
			{
				guardSlow(DoCopy);
				checkSlow(&(*this)(0)!=NULL);
				checkSlow(ArrayNum*sizeof(TCHAR)>=0);
				appMemcpy( &(*this)(0), *Other, ArrayNum*sizeof(TCHAR) );
				unguardSlow;
			}
		}
		return *this;
		unguardSlow;
	}
	TCHAR& operator[]( INT i )
	{
		guardSlow(FString::operator());
		checkSlow(i>=0);
		checkSlow(i<ArrayNum);
		checkSlow(ArrayMax>=ArrayNum);
		return ((TCHAR*)Data)[i];
		unguardSlow;
	}
	const TCHAR& operator[]( INT i ) const
	{
		guardSlow(FString::operator());
		checkSlow(i>=0);
		checkSlow(i<ArrayNum);
		checkSlow(ArrayMax>=ArrayNum);
		return ((TCHAR*)Data)[i];
		unguardSlow;
	}
	~FString()
	{
		TArray<TCHAR>::Empty();		
	}
	void Empty()
	{
		TArray<TCHAR>::Empty();
	}
	void Shrink()
	{
		TArray<TCHAR>::Shrink();
	}
	const TCHAR* operator*() const
	{
		return Num() ? &(*this)(0) : TEXT("");
	}
	operator UBOOL() const
	{
		return Num()!=0;
	}
	TArray<TCHAR>& GetCharArray()
	{
		//warning: Operations on the TArray<CHAR> can be unsafe, such as adding
		// non-terminating 0's or removing the terminating zero.
		return (TArray<TCHAR>&)*this;
	}
	FString& operator+=( const TCHAR* Str )
	{
		guardSlow(FString& operator+=( const TCHAR* Str ))
		if ( Str && *Str!='\0' )
		{
			if( ArrayNum )
			{
				INT Index = ArrayNum-1;
				Add( appStrlen(Str) );
				appStrcpy( &(*this)(Index), Str );
			}
			else if( *Str )
			{
				Add( appStrlen(Str)+1 );
				appStrcpy( &(*this)(0), Str );
			}
		}
		return *this;
		unguardSlow;
	}
	FString& operator+=( const FString& Str )
	{
		return operator+=( *Str );
	}
	FString operator+( const TCHAR* Str )
	{
		return FString( *this ) += Str;
	}
	FString operator+( const FString& Str )
	{
		return operator+( *Str );
	}
	FString& operator*=( const TCHAR* Str )
	{
		if( ArrayNum>1 && (*this)(ArrayNum-2)!=PATH_SEPARATOR[0] )
			*this += PATH_SEPARATOR;
		return *this += Str;
	}
	FString& operator*=( const FString& Str )
	{
		return operator*=( *Str );
	}
	FString operator*( const TCHAR* Str ) const
	{
		return FString( *this ) *= Str;
	}
	FString operator*( const FString& Str ) const
	{
		return operator*( *Str );
	}
	UBOOL operator<=( const TCHAR* Other ) const
	{
		return !(appStricmp( **this, Other ) > 0);
	}
	UBOOL operator<( const TCHAR* Other ) const
	{
		return appStricmp( **this, Other ) < 0;
	}
	UBOOL operator>=( const TCHAR* Other ) const
	{
		return !(appStricmp( **this, Other ) < 0);
	}
	UBOOL operator>( const TCHAR* Other ) const
	{
		return appStricmp( **this, Other ) > 0;
	}
	UBOOL operator==( const TCHAR* Other ) const
	{
		return appStricmp( **this, Other )==0;
	}
	UBOOL operator==( const FString& Other ) const
	{
		return appStricmp( **this, *Other )==0;
	}
	UBOOL operator!=( const TCHAR* Other ) const
	{
		return appStricmp( **this, Other )!=0;
	}
	UBOOL operator!=( const FString& Other ) const
	{
		return appStricmp( **this, *Other )!=0;
	}
	INT Len() const
	{
		return Num() ? Num()-1 : 0;
	}
	FString Left( INT Count ) const
	{
		return FString( Clamp(Count,0,Len()), **this );
	}
	FString LeftChop( INT Count ) const
	{
		return FString( Clamp(Len()-Count,0,Len()), **this );
	}
	FString Right( INT Count ) const
	{
		return FString( **this + Len()-Clamp(Count,0,Len()) );
	}
	FString Mid( INT Start, INT Count=MAXINT ) const // !! FIX-ME !! 
	{
		DWORD End = Start+Count;
		Start    = Clamp<DWORD>( Start, 0,     Len() );
		End      = Clamp<DWORD>( End,   Start, Len() );
		return FString( End-Start, **this + Start );
	}
	FString FStringReplace( FString Replace, FString With ) const
	{
		int i;
		FString InputT;
		FString Text( **this );
		
		InputT = Text;
		Text = TEXT("");
		i = InputT.InStr(Replace);
		while(i != -1)
		{
			Text = Text + InputT.Left(i) + With;
			InputT = InputT.Mid(i + Replace.Len());
			i = InputT.InStr(Replace);
		}
		Text = Text+ InputT;
		return Text;
	}
	INT InStr( const TCHAR* SubStr, UBOOL Right=0 ) const
	{
		if( !Right )
		{
			const TCHAR* Tmp = appStrstr(**this,SubStr);
			return Tmp ? (Tmp-**this) : -1;
		}
		else
		{
			for( INT i=Len()-1; i>=0; i-- )
			{
				INT j;
				for( j=0; SubStr[j]; j++ )
					if( (*this)(i+j)!=SubStr[j] )
						break;
				if( !SubStr[j] )
					return i;
			}
			return -1;
		}
	}
	INT InStr( const FString& SubStr, UBOOL Right=0 ) const
	{
		return InStr( *SubStr, Right );
	}
	UBOOL Split( const FString& InS, FString* LeftS, FString* RightS, UBOOL Right=0 ) const
	{
		INT InPos = InStr(InS,Right);
		if( InPos<0 )
			return 0;
		if( LeftS )
			*LeftS = Left(InPos);
		if( RightS )
			*RightS = Mid(InPos+InS.Len());
		return 1;
	}
	FString Caps() const
	{
		FString New( **this );
		for( INT i=0; i<ArrayNum; i++ )
			New(i) = appToUpper(New(i));
		return New;
	}
	FString Locs() const
	{
		FString New( **this );
		for( INT i=0; i<ArrayNum; i++ )
			New(i) = appToLower(New(i));
		return New;
	}
	INT Int() const
	{
		guardSlow(FString::Int);
		return appAtoi( **this );
		unguardSlow;
	}
	FLOAT Float() const
	{
		guardSlow(FString::Float);
		return appAtof( **this );
		unguardSlow;
	}


	UBOOL IsAlpha()
	{
		guard(FString::IsAlpha);
		for ( INT Pos=0; Pos<Len(); Pos++ )
			if ( !appIsAlpha((*this)(Pos)) )
				return 0;
		return 1;
		unguard;
	}
	UBOOL IsAlnum()
	{
		guard(FString::IsAlnum);
		for ( INT Pos=0; Pos<Len(); Pos++ )
			if ( !appIsAlnum((*this)(Pos)) )
				return 0;
		return 1;
		unguard;
	}
	UBOOL IsName( UBOOL AllowDigitStart=0 )
	{
		guard(FString::IsName);
		if ( Len()==0 )
			return 1;
		if ( !AllowDigitStart && appIsDigit((*this)(0)) )
			return 0;
		for ( INT Pos=0; Pos<Len(); Pos++ )
			if ( !appIsAlnum((*this)(Pos)) && (*this)(Pos)!='_' )
				return 0;
		return 1;
		unguard;
	}
	const TCHAR* DynamicString() const
	{
		guard(FString::DynamicString);
		return appStrncpy( appDynamicString(Len()+1), **this, Len()+1 );
		unguard;
	}
	CORE_API FString LeftPad( INT ChCount );
	CORE_API FString RightPad( INT ChCount );
	CORE_API static FString Printf( const TCHAR* Fmt, ... ); // CoreI will provide fixed version.
	CORE_API static FString Chr( TCHAR Ch );
	CORE_API friend FArchive& operator<<( FArchive& Ar, FString& S );
	friend class FStringNoInit;
private:
	FString( INT InCount, const TCHAR* InSrc )
	:	TArray<TCHAR>( InCount ? InCount+1 : 0 )
	{
		if( ArrayNum )
			appStrncpy( &(*this)(0), InSrc, InCount+1 );
	}
};
class FStringNoInit : public FString
{
public:
	FStringNoInit()
	: FString( E_NoInit )
	{}
	FStringNoInit& operator=( const TCHAR* Other )
	{
		guardSlow(FString:: operator=(const TCHAR*));
		if( Data==NULL || &(*this)(0)!=Other )
		{
			ArrayNum = ArrayMax = (Other && *Other) ? appStrlen(Other)+1 : 0;
			Realloc( sizeof(TCHAR) );
			if( ArrayNum )
				appMemcpy( &(*this)(0), Other, ArrayNum*sizeof(TCHAR) );
		}
		return *this;
		unguardSlow;
	}
	FStringNoInit& operator=( const FString& Other )
	{
		if( Data==NULL || this!=&Other )
		{
			ArrayNum = ArrayMax = Other.Num();
			Realloc( sizeof(TCHAR) );
			if( ArrayNum && &(*this)(0) )
				appMemcpy( &(*this)(0), *Other, ArrayNum*sizeof(TCHAR) );
		}
		return *this;
	}
};
inline DWORD GetTypeHash( const FString& S )
{
	return appStrihash(*S);
}
template <> struct TTypeInfo<FString> : public TTypeInfoBase<FString>
{
	typedef const TCHAR* ConstInitType;
	static const TCHAR* ToInit( const FString& In ) {return *In;}
	static UBOOL DefinitelyNeedsDestructor() {return 0;}
};

//
// String exchanger.
//
inline void ExchangeString( FString& A, FString& B )
{
	guardSlow(ExchangeTArray);
	appMemswap( &A, &B, sizeof(FString) );
	unguardSlow;
}

#define FSTRING(str) FString(TEXT(str))

/*----------------------------------------------------------------------------
	Special archivers.
----------------------------------------------------------------------------*/

//
// String output device.
//
class FStringOutputDevice : public FString, public FOutputDevice
{
public:
	FStringOutputDevice( const TCHAR* InStr=TEXT("") )
	: FString( InStr )
	{}
	void Serialize( const TCHAR* InData, EName Event )
	{
		*this += (TCHAR*)InData;
	}
};

//
// Buffer writer.
//
class FBufferWriter : public FArchive
{
public:
	FBufferWriter( TArray<BYTE>& InBytes )
	: Bytes( InBytes )
	, Pos( 0 )
	{
		ArIsSaving = 1;
	}
	void Serialize( void* InData, INT Length )
	{
		if ( Pos+Length>Bytes.Num() )
			Bytes.Add( Pos+Length-Bytes.Num() );
		if ( Length==1 )
			Bytes(Pos) = ((BYTE*)InData)[0];
		else if ( Length>0 )
			appMemcpy( &Bytes(Pos), InData, Length );
		Pos += Length;
	}
	INT Tell()
	{
		return Pos;
	}
	void Seek( INT InPos )
	{
		Pos = InPos;
	}
	INT TotalSize()
	{
		return Bytes.Num();
	}
private:
	TArray<BYTE>& Bytes;
	INT Pos;
};

//
// Buffer archiver.
//
class FBufferArchive : public FBufferWriter, public TArray<BYTE>
{
public:
	FBufferArchive()
	: FBufferWriter( (TArray<BYTE>&)*this )
	{}
};

//
// Buffer reader.
//
class FBufferReader : public FArchive
{
public:
	FBufferReader( const TArray<BYTE>& InBytes )
	:	Bytes	( InBytes )
	,	Pos 	( 0 )
	{
		ArIsLoading = ArIsTrans = 1;
	}
	void Serialize( void* Data, INT Num )
	{
		checkSlow(Pos>=0);
		checkSlow(Pos+Num<=Bytes.Num());
		if ( Num==1 )
			((BYTE*)Data)[0] = Bytes(Pos);
		else if ( Num>0 )
			appMemcpy( Data, &Bytes(Pos), Num );
		Pos += Num;
	}
	INT Tell()
	{
		return Pos;
	}
	INT TotalSize()
	{
		return Bytes.Num();
	}
	void Seek( INT InPos )
	{
		check(InPos>=0);
		check(InPos<=Bytes.Num());
		Pos = InPos;
	}
	UBOOL AtEnd()
	{
		return Pos>=Bytes.Num();
	}
private:
	const TArray<BYTE>& Bytes;
	INT Pos;
};

/*----------------------------------------------------------------------------
	TMap.
----------------------------------------------------------------------------*/

//
// Maps unique keys to values.
//
template< class TK, class TI > class TMapBase
{
protected:
	class TPair
	{
	public:
		INT HashNext;
		TK Key;
		TI Value;
		TPair( typename TTypeInfo<TK>::ConstInitType InKey, typename TTypeInfo<TI>::ConstInitType InValue )
		: Key( InKey ), Value( InValue )
		{}
		TPair()
		{}
		friend FArchive& operator<<( FArchive& Ar, TPair& F )
		{
			guardSlow(TMapBase::TPair<<);
			return Ar << F.Key << F.Value;
			unguardSlow;
		}
	};
	void Rehash()
	{
		guardSlow(TMapBase::Rehash);
		checkSlow(!(HashCount&(HashCount-1)));
		checkSlow(HashCount>=8);
		INT* NewHash = new(TEXT("HashMapHash"))INT[HashCount];
		{for( INT i=0; i<HashCount; i++ )
		{
			NewHash[i] = INDEX_NONE;
		}}
		{for( INT i=0; i<Pairs.Num(); i++ )
		{
			TPair& Pair    = Pairs(i);
			INT    iHash   = (GetTypeHash(Pair.Key) & (HashCount-1));
			Pair.HashNext  = NewHash[iHash];
			NewHash[iHash] = i;
		}}
		if ( Hash )
			delete[] Hash;
		Hash = NewHash;
		unguardSlow;
	}
	void Relax()
	{
		guardSlow(TMapBase::Relax);
		while( HashCount>Pairs.Num()*2+8 )
			HashCount /= 2;
		Rehash();
		unguardSlow;
	}
	TI& Add( typename TTypeInfo<TK>::ConstInitType InKey, typename TTypeInfo<TI>::ConstInitType InValue )
	{
		guardSlow(TMapBase::Add);
		TPair& Pair   = *new(Pairs)TPair( InKey, InValue );
		INT    iHash  = (GetTypeHash(Pair.Key) & (HashCount-1));
		Pair.HashNext = Hash[iHash];
		Hash[iHash]   = Pairs.Num()-1;
		if( HashCount*2+8 < Pairs.Num() )
		{
			HashCount *= 2;
			Rehash();
		}
		return Pair.Value;
		unguardSlow;
	}
	TArray<TPair> Pairs;
	INT* Hash;
	INT HashCount;
public:
	TMapBase()
	:	Hash( NULL )
	,	HashCount( 8 )
	{
		guardSlow(TMapBase::TMapBase);
		Rehash();
		unguardSlow;
	}
	TMapBase( const TMapBase& Other )
	:	Pairs( Other.Pairs )
	,	HashCount( Other.HashCount )
	,	Hash( NULL )
	{
		guardSlow(TMapBase::TMapBase copy);
		Rehash();
		unguardSlow;
	}
	~TMapBase()
	{
		if( Hash )
			delete[] Hash;
		Hash = NULL;
		HashCount = 0;
	}
	TMapBase& operator=( const TMapBase& Other )
	{
		guardSlow(TMapBase::operator=);
		Pairs     = Other.Pairs;
		HashCount = Other.HashCount;
		Rehash();
		return *this;
		unguardSlow;
	}
	void Empty()
	{
		guardSlow(TMapBase::Empty);
		checkSlow(!(HashCount&(HashCount-1)));
		Pairs.Empty();
		HashCount = 8;
		Rehash();
		unguardSlow;
	}
	TI& Set( typename TTypeInfo<TK>::ConstInitType InKey, typename TTypeInfo<TI>::ConstInitType InValue )
	{
		guardSlow(TMap::Set);
		for( INT i=Hash[(GetTypeHash(InKey) & (HashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==InKey )
				{Pairs(i).Value=InValue; return Pairs(i).Value;}
		return Add( InKey, InValue );
		unguardSlow;
	}
	INT Remove( typename TTypeInfo<TK>::ConstInitType InKey )
	{
		guardSlow(TMapBase::Remove);
		INT Count=0;
		for( INT i=Pairs.Num()-1; i>=0; i-- )
			if( Pairs(i).Key==InKey )
				{Pairs.Remove(i); Count++;}
		if( Count )
			Relax();
		return Count;
		unguardSlow;
	}
	TI* Find( const TK& Key )
	{
		guardSlow(TMapBase::Find);
		for( INT i=Hash[(GetTypeHash(Key) & (HashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				return &Pairs(i).Value;
		return NULL;
		unguardSlow;
	}
	TI FindRef( const TK& Key )
	{
		guardSlow(TMapBase::Find);
		for( INT i=Hash[(GetTypeHash(Key) & (HashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				return Pairs(i).Value;
		return NULL;
		unguardSlow;
	}
	const TI* Find( const TK& Key ) const
	{
		guardSlow(TMapBase::Find);
		for( INT i=Hash[(GetTypeHash(Key) & (HashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				return &Pairs(i).Value;
		return NULL;
		unguardSlow;
	}
	friend FArchive& operator<<( FArchive& Ar, TMapBase& M )
	{
		guardSlow(TMapBase<<);
		Ar << M.Pairs;
		if( Ar.IsLoading() )
			M.Rehash();
		return Ar;
		unguardSlow;
	}
	void Dump( FOutputDevice& Ar )
	{
		guard(TMapBase::Dump);
#if 1
		// Give some more useful statistics. --han
		INT NonEmpty = 0, Worst = 0;
		for( INT i=0; i<HashCount; i++ )
		{
			INT c=0;
			for( INT j=Hash[i]; j!=INDEX_NONE; j=Pairs(j).HashNext )
				c++;
			if ( c>Worst )
				Worst = c;
			if ( c>0 )
			{
				NonEmpty++;
				Ar.Logf( TEXT("   Hash[%i] = %i"), i, c );
			}
		}
		Ar.Logf( TEXT("TMapBase: %i items, worst %i, %i/%i hash slots used."), Pairs.Num(), Worst, NonEmpty, HashCount );
#else
		// oldver.
		Ar.Logf( TEXT("TMapBase: %i items, %i hash slots"), Pairs.Num(), HashCount );
		for( INT i=0; i<HashCount; i++ )
		{
			INT c=0;
			for( INT j=Hash[i]; j!=INDEX_NONE; j=Pairs(j).HashNext )
				c++;
			Ar.Logf( TEXT("   Hash[%i] = %i"), i, c );
		}
#endif
		unguard;
	}
	class TIterator
	{
	public:
		TIterator( TMapBase<TK,TI>& InMap ) : Pairs( InMap.Pairs ), Index( 0 ) {}
		void operator++()          { ++Index; }
		void Increment()           { ++Index; }
		void RemoveCurrent()       { Pairs.Remove(Index--); }
		operator UBOOL() const     { return Index<Pairs.Num(); }
		TK& Key() const            { return Pairs(Index).Key; }
		TI& Value() const          { return Pairs(Index).Value; }
	private:
		TArray<TPair>& Pairs;
		INT Index;
		friend class TUniqueKeyIterator;
	};
	friend class TIterator;
};
template< class TK, class TI > class TMap : public TMapBase<TK,TI>
{
public:
	TMap& operator=( const TMap& Other )
	{
		TMapBase<TK,TI>::operator=( Other );
		return *this;
	}
	INT Num()
	{
		guardSlow(TMap::Num);
		return this->Pairs.Num();
		unguardSlow;
	}
};
template< class TK, class TI > class TMultiMap : public TMapBase<TK,TI>
{
public:
	TMultiMap& operator=( const TMultiMap& Other )
	{
		TMapBase<TK,TI>::operator=( Other );
		return *this;
	}
	INT Num()
	{
		guardSlow(TMultiMap::Num);
		return Pairs.Num();
		unguardSlow;
	}
	INT Num( TArray<TK>& Keys )
	{
		guardSlow(TMultiMap::Num);
		Keys.Empty();

		for( INT i=0;i < this->Pairs.Num();i++ )
		{
			INT j=Keys.FindItemIndex(Pairs(i).Key);
			if (j==INDEX_NONE)
			{
				j=Keys.AddZeroed();
				Keys(j)=Pairs(i).Key;
			}
		}
		return Keys.Num();
		unguardSlow;
	}
	void MultiFind( const TK& Key, TArray<TI>& Values )
	{
		guardSlow(TMultiMap::MultiFind);
		for( INT i=Hash[(GetTypeHash(Key) & (HashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key )
				new(Values)TI(Pairs(i).Value);
		unguardSlow;
	}
	TI& Add( typename TTypeInfo<TK>::ConstInitType InKey, typename TTypeInfo<TI>::ConstInitType InValue )
	{
		return TMapBase<TK,TI>::Add( InKey, InValue );
	}
	TI& AddUnique( typename TTypeInfo<TK>::ConstInitType InKey, typename TTypeInfo<TI>::ConstInitType InValue )
	{
		for( INT i=Hash[(GetTypeHash(InKey) & (HashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==InKey && Pairs(i).Value==InValue )
				return Pairs(i).Value;
		return Add( InKey, InValue );
	}
	INT MultiRemove( const TK& InKey )
	{
		guardSlow(TMultiMap::RemoveMulti);
		INT Count = 0;
		for ( INT i=Pairs.Num()-1; i>=0; i-- )
			if ( Pairs(i).Key == InKey )
				{Pairs.Remove(i); Count++;}
		if ( Count )
			this->Relax();
		return Count;
		unguardSlow;
	}
	INT RemovePair( typename TTypeInfo<TK>::ConstInitType InKey, typename TTypeInfo<TI>::ConstInitType InValue )
	{
		guardSlow(TMultiMap::Remove);
		INT Count=0;
		for( INT i=Pairs.Num()-1; i>=0; i-- )
			if( Pairs(i).Key==InKey && Pairs(i).Value==InValue )
				{Pairs.Remove(i); Count++;}
		if( Count )
			Relax();
		return Count;
		unguardSlow;
	}
	TI* FindPair( const TK& Key, const TK& Value )
	{
		guardSlow(TMultiMap::Find);
		for( INT i=Hash[(GetTypeHash(Key) & (HashCount-1))]; i!=INDEX_NONE; i=Pairs(i).HashNext )
			if( Pairs(i).Key==Key && Pairs(i).Value==Value )
				return &Pairs(i).Value;
		return NULL;
		unguardSlow;
	}
	class TUniqueKeyIterator : public TMapBase<TK,TI>::TIterator
	{
	public:
		TUniqueKeyIterator( TMapBase<TK,TI>& InMap )
		: TMapBase<TK,TI>::TIterator(InMap)
		{
			if ( *this )
				VisitedKeys.AddUniqueItem( Key() );
		}
		void operator++()
		{
			while ( 1 )
			{
				Increment();

				if ( !*this )
					return;

				INT OldNum = VisitedKeys.Num();
				VisitedKeys.AddUniqueItem(Key());
				if ( OldNum!=VisitedKeys.Num() )
					return;
			}
		}
	private:
		TArray<TK> VisitedKeys;
	};
	friend class TUniqueKeyIterator;
};

// !! TMapNoInit !!
// !! TMultiMapNoInit !!

/*----------------------------------------------------------------------------
	Sorting template.
----------------------------------------------------------------------------*/

//
// Sort elements. The sort is unstable, meaning that the ordering of equal 
// items is not necessarily preserved.
//
template<class T> struct TStack
{
	T* Min;
	T* Max;
};
template<class T> void Sort( T* First, INT Num )
{
	guard(Sort);
	if( Num<2 )
		return;
	TStack<T> RecursionStack[32]={{First,First+Num-1}}, Current, Inner;
	for( TStack<T>* StackTop=RecursionStack; StackTop>=RecursionStack; --StackTop )
	{
		Current = *StackTop;
	Loop:
		INT Count = Current.Max - Current.Min + 1;
		if( Count <= 8 )
		{
			// Use simple bubble-sort.
			while( Current.Max > Current.Min )
			{
				T *Max, *Item;
				for( Max=Current.Min, Item=Current.Min+1; Item<=Current.Max; Item++ )
					if( Compare(*Item, *Max) > 0 )
						Max = Item;
				Exchange( *Max, *Current.Max-- );
			}
		}
		else
		{
			// Grab middle element so sort doesn't exhibit worst-cast behaviour with presorted lists.
			Exchange( Current.Min[Count/2], Current.Min[0] );

			// Divide list into two halves, one with items <=Current.Min, the other with items >Current.Max.
			Inner.Min = Current.Min;
			Inner.Max = Current.Max+1;
			for( ; ; )
			{
				while( ++Inner.Min<=Current.Max && Compare(*Inner.Min, *Current.Min) <= 0 );
				while( --Inner.Max> Current.Min && Compare(*Inner.Max, *Current.Min) >= 0 );
				if( Inner.Min>Inner.Max )
					break;
				Exchange( *Inner.Min, *Inner.Max );
			}
			Exchange( *Current.Min, *Inner.Max );

			// Save big half and recurse with small half.
			if( Inner.Max-1-Current.Min >= Current.Max-Inner.Min )
			{
				if( Current.Min+1 < Inner.Max )
				{
					StackTop->Min = Current.Min;
					StackTop->Max = Inner.Max - 1;
					StackTop++;
				}
				if( Current.Max>Inner.Min )
				{
					Current.Min = Inner.Min;
					goto Loop;
				}
			}
			else
			{
				if( Current.Max>Inner.Min )
				{
					StackTop->Min = Inner  .Min;
					StackTop->Max = Current.Max;
					StackTop++;
				}
				if( Current.Min+1<Inner.Max )
				{
					Current.Max = Inner.Max - 1;
					goto Loop;
				}
			}
		}
	}
	unguard;
}

/*----------------------------------------------------------------------------
	TDoubleLinkedList.
----------------------------------------------------------------------------*/

//
// Simple double-linked list template.
//
template< class T > class TDoubleLinkedList : public T
{
public:
	TDoubleLinkedList* Next;
	TDoubleLinkedList** PrevLink;
	void Unlink()
	{
		if( Next )
			Next->PrevLink = PrevLink;
		*PrevLink = Next;
	}
	void Link( TDoubleLinkedList*& Before )
	{
		if( Before )
			Before->PrevLink = &Next;
		Next     = Before;
		PrevLink = &Before;
		Before   = this;
	}
};

/*----------------------------------------------------------------------------
	TList.
----------------------------------------------------------------------------*/

//
// Simple single-linked list template.
//
template <class ElementType> class TList
{
public:

	ElementType			Element;
	TList<ElementType>*	Next;

	// Constructor.

	TList(ElementType InElement,TList<ElementType>* InNext = NULL)
	{
		Element = InElement;
		Next = InNext;
	}
};

/*----------------------------------------------------------------------------
	FRainbowPtr.
----------------------------------------------------------------------------*/

//
// A union of pointers of all base types.
//
union FRainbowPtr
{
	// All pointers.
	void*  PtrVOID;
	BYTE*  PtrBYTE;
	_WORD* PtrWORD;
	DWORD* PtrDWORD;
	QWORD* PtrQWORD;
	FLOAT* PtrFLOAT;

	// Conversion constructors.
	FRainbowPtr() {}
	FRainbowPtr( void* Ptr ) : PtrVOID(Ptr) {};
};

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
