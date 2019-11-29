/*=============================================================================
	UnMath.h: Unreal math routines
	Copyright 1997-1999 Epic Games, Inc. All Rights Reserved.

	Revision history:
		* Created by Tim Sweeney
=============================================================================*/

/*-----------------------------------------------------------------------------
	Defintions.
-----------------------------------------------------------------------------*/

// Forward declarations.
class FVector;
class FPlane;
class FCoords;
class FRotator;
class FScale;
class FGlobalMath;


// Fixed point conversion.
inline INT Fix   ( INT   A ) { return A<<16; };
inline INT Fix   ( FLOAT A ) { return appRound(A*65536.f); };
inline INT Unfix ( INT   A ) { return A>>16; };

// Constants.
#undef  PI
#define PI 					(3.1415926535897932f)
#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)

// Magic numbers for numerical precision.
#define DELTA			(0.00001f)
#define SLERP_DELTA		(0.0001f)

/*-----------------------------------------------------------------------------
	Global functions.
-----------------------------------------------------------------------------*/

//
// Snap a value to the nearest grid multiple.
//
inline FLOAT FSnap( FLOAT Location, FLOAT Grid )
{
	if ( Grid==0.f )
		return Location;
	else
		return appFloor((Location + 0.5f*Grid)/Grid)*Grid;
}

//
// Internal sheer adjusting function so it snaps nicely at 0 and 45 degrees.
//
inline FLOAT FSheerSnap( FLOAT Sheer )
{
	if      ( Sheer<-0.65f ) return Sheer+0.15f;
	else if ( Sheer>+0.65f ) return Sheer-0.15f;
	else if ( Sheer<-0.55f ) return      -0.50f;
	else if ( Sheer>+0.55f ) return      +0.50f;
	else if ( Sheer<-0.05f ) return Sheer+0.05f;
	else if ( Sheer>+0.05f ) return Sheer-0.05f;
	else                     return       0.00f;
}

//
// Find the closest power of 2 that is >= N.
//
inline DWORD FNextPowerOfTwo( DWORD N )
{
	if ( N<=0L ) return 0L;
	if ( N<=1L ) return 1L;
	if ( N<=2L ) return 2L;
	if ( N<=4L ) return 4L;
	if ( N<=8L ) return 8L;
	if ( N<=16L ) return 16L;
	if ( N<=32L ) return 32L;
	if ( N<=64L ) return 64L;
	if ( N<=128L ) return 128L;
	if ( N<=256L ) return 256L;
	if ( N<=512L ) return 512L;
	if ( N<=1024L ) return 1024L;
	if ( N<=2048L ) return 2048L;
	if ( N<=4096L ) return 4096L;
	if ( N<=8192L ) return 8192L;
	if ( N<=16384L ) return 16384L;
	if ( N<=32768L ) return 32768L;
	if ( N<=65536L ) return 65536L;

	return 0;
}

//
// Add to a word angle, constraining it within a min (not to cross)
// and a max (not to cross).  Accounts for funkyness of word angles.
// Assumes that angle is initially in the desired range.
//
inline _WORD FAddAngleConfined( INT Angle, INT Delta, INT MinThresh, INT MaxThresh )
{
	if ( Delta<0 )
	{
		if ( Delta<=-0x10000L || Delta<=-(INT)((_WORD)(Angle-MinThresh)) )
			return (_WORD)MinThresh;
	}
	else if ( Delta > 0 )
	{
		if ( Delta>=0x10000L || Delta>=(INT)((_WORD)(MaxThresh-Angle)) )
			return (_WORD)MaxThresh;
	}
	return (_WORD)(Angle+Delta);
}

//
// Eliminate all fractional precision from an angle.
//
// inlined version below which depends on FGlobalMath.
//
INT ReduceAngle( INT Angle );

/*-----------------------------------------------------------------------------
	FVector.
-----------------------------------------------------------------------------*/

// Information associated with a floating point vector, describing its
// status as a point in a rendering context.
enum EVectorFlags
{
	FVF_OutXMin   = 0x04,	// Outcode rejection, off left hand side of screen.
	FVF_OutXMax   = 0x08,	// Outcode rejection, off right hand side of screen.
	FVF_OutYMin   = 0x10,	// Outcode rejection, off top of screen.
	FVF_OutYMax   = 0x20,	// Outcode rejection, off bottom of screen.
	FVF_OutNear   = 0x40, // Near clipping plane.
	FVF_OutFar    = 0x80, // Far clipping plane.
	FVF_OutReject = (FVF_OutXMin | FVF_OutXMax | FVF_OutYMin | FVF_OutYMax), // Outcode rejectable.
	FVF_OutSkip   = (FVF_OutXMin | FVF_OutXMax | FVF_OutYMin | FVF_OutYMax), // Outcode clippable.

	FVF_MAX       = 0xFF,
};

inline FString appVectorFlagsString( const BYTE VectorFlags )
{
	guard(appVectorFlagsString);
	if ( VectorFlags==FVF_MAX )
		return TEXT("FVF_MAX");

	BYTE Flags = VectorFlags;
	FString Result;

	if ( (Flags&FVF_OutReject)==FVF_OutReject )
	{
		Flags &= ~FVF_OutReject;

		Result += TEXT("OutReject");
	}

	if ( Flags&FVF_OutXMin   ) { Flags &= ~FVF_OutXMin; if ( Result.Len() ) Result += TEXT("| "); Result += TEXT("OutXMin"  ); }
	if ( Flags&FVF_OutXMax   ) { Flags &= ~FVF_OutXMax; if ( Result.Len() ) Result += TEXT("| "); Result += TEXT("OutXMax"  ); }
	if ( Flags&FVF_OutYMin   ) { Flags &= ~FVF_OutYMin; if ( Result.Len() ) Result += TEXT("| "); Result += TEXT("OutYMin"  ); }
	if ( Flags&FVF_OutYMax   ) { Flags &= ~FVF_OutYMax; if ( Result.Len() ) Result += TEXT("| "); Result += TEXT("OutYMax"  ); }
	if ( Flags&FVF_OutNear   ) { Flags &= ~FVF_OutNear; if ( Result.Len() ) Result += TEXT("| "); Result += TEXT("OutNear"  ); }
	if ( Flags&FVF_OutFar    ) { Flags &= ~FVF_OutFar;  if ( Result.Len() ) Result += TEXT("| "); Result += TEXT("OutFar"   ); }

	if ( Flags )
	{
		if ( Result.Len() )
			Result += TEXT("| ");

		Result += FString::Printf( TEXT("0x%02X"), Flags );
	}
	return Result;
	unguard;
}

//
// Floating point vector.
//
class FVector 
{
public:

	// Variables.
	union
	{
		struct {FLOAT X,Y,Z;}; // Coordinates.
		struct {FLOAT R,G,B;}; // Color components.
		struct {FLOAT S,T,P;}; // The other thing.
		struct {FLOAT U,V,W;}; // The other other thing.
	};

	// Constructors.
	FVector()
	{}
	FVector( FLOAT InX, FLOAT InY, FLOAT InZ )
	:	X(InX)
	, Y(InY)
	, Z(InZ)
	{}

	// Binary math operators.
	FVector operator^( const FVector& V ) const
	{
		return FVector
		(
			Y*V.Z-Z*V.Y,
			Z*V.X-X*V.Z,
			X*V.Y-Y*V.X
		);
	}
	FLOAT operator|( const FVector& V ) const
	{
		return X*V.X+Y*V.Y+Z*V.Z;
	}
	friend FVector operator*( FLOAT Scale, const FVector& V )
	{
		return FVector( V.X*Scale, V.Y*Scale, V.Z*Scale );
	}
	FVector operator+( const FVector& V ) const
	{
		return FVector( X+V.X, Y+V.Y, Z+V.Z );
	}
	FVector operator-( const FVector& V ) const
	{
		return FVector( X-V.X, Y-V.Y, Z-V.Z );
	}
	FVector operator*( FLOAT Scale ) const
	{
		return FVector( X*Scale, Y*Scale, Z*Scale );
	}
	FVector operator/( FLOAT Scale ) const
	{
		return FVector( X/Scale, Y/Scale, Z/Scale );
	}
	FVector operator*( const FVector& V ) const
	{
		return FVector( X*V.X, Y*V.Y, Z*V.Z );
	}

	// Binary comparison operators.
	UBOOL operator==( const FVector& V ) const
	{
		return X==V.X && Y==V.Y && Z==V.Z;
	}
	UBOOL operator!=( const FVector& V ) const
	{
		return X!=V.X || Y!=V.Y || Z!=V.Z;
	}

	// Unary operators.
	FVector operator-() const
	{
		return FVector( -X, -Y, -Z );
	}

	// Assignment operators.
	FVector operator+=( const FVector& V )
	{
		X += V.X;
		Y += V.Y;
		Z += V.Z;

		return *this;
	}
	FVector operator-=( const FVector& V )
	{
		X -= V.X;
		Y -= V.Y;
		Z -= V.Z;

		return *this;
	}
	FVector operator*=( FLOAT Scale )
	{
		X *= Scale;
		Y *= Scale;
		Z *= Scale;

		return *this;
	}
	FVector operator/=( FLOAT V )
	{
		X /= Scale;
		Y /= Scale;
		Z /= Scale;

		return *this;
	}
	FVector operator*=( const FVector& V )
	{
		X *= V.X;
		Y *= V.Y;
		Z *= V.Z;

		return *this;
	}
	FVector operator/=( const FVector& V )
	{
		X /= V.X;
		Y /= V.Y;
		Z /= V.Z;

		return *this;
	}

	// Simple functions.
	FLOAT Size() const
	{
		return appSqrt( SizeSquared() );
	}
	FLOAT SizeFast() const
	{
		return 1.f/appFastInvSqrt(SizeSquared());
	}
	FLOAT SizeFastN0() const
	{
		return 1.f/appFastInvSqrtN0(SizeSquared());
	}
	FLOAT SizeFastN1() const
	{
		return 1.f/appFastInvSqrtN1(SizeSquared());
	}
	FLOAT SizeFastN2() const
	{
		return 1.f/appFastInvSqrtN2(SizeSquared());
	}
	FLOAT SizeFastN3() const
	{
		return 1.f/appFastInvSqrtN3(SizeSquared());
	}
	FLOAT SizeSquared() const
	{
		return X*X+Y*Y+Z*Z;
	}
	FLOAT Size2D() const 
	{
		return appSqrt( X*X + Y*Y );
	}
	FLOAT Size2DFast() const 
	{
		return 1.f/appFastInvSqrt( SizeSquared2D() );
	}
	FLOAT Size2DFastN0() const 
	{
		return 1.f/appFastInvSqrtN0( SizeSquared2D() );
	}
	FLOAT Size2DFastN1() const 
	{
		return 1.f/appFastInvSqrtN1( SizeSquared2D() );
	}
	FLOAT Size2DFastN2() const 
	{
		return 1.f/appFastInvSqrtN2( SizeSquared2D() );
	}
	FLOAT Size2DFastN3() const 
	{
		return 1.f/appFastInvSqrtN3( SizeSquared2D() );
	}
	FLOAT SizeSquared2D() const 
	{
		return X*X+Y*Y;
	}
	UBOOL IsNearlyZero() const
	{
		return Abs<FLOAT>(X)<KINDA_SMALL_NUMBER && Abs<FLOAT>(Y)<KINDA_SMALL_NUMBER && Abs<FLOAT>(Z)<KINDA_SMALL_NUMBER;
	}
	UBOOL IsZero() const
	{
		return X==0.0 && Y==0.0 && Z==0.0;
	}
	UBOOL Normalize()
	{
		FLOAT SquareSum = SizeSquared();
		if ( SquareSum>=SMALL_NUMBER )
		{
			FLOAT Scale = 1.0/appSqrt(SquareSum);
			
			X *= Scale;
			Y *= Scale;
			Z *= Scale;

			return 1;
		}
		return 0;
	}
	FVector Projection() const
	{
		return FVector( X/Z, Y/Z, 1.0f );
	}
	FVector UnsafeNormal() const
	{
		FLOAT Scale = 1.0/Size();
		return FVector( X*Scale, Y*Scale, Z*Scale );
	}
	FVector UnsafeNormalFast() const
	{
		FLOAT Scale = 1.0/SizeFast();
		return FVector( X*Scale, Y*Scale, Z*Scale );
	}
	FVector GridSnap( const FVector& Grid )
	{
		return FVector( FSnap(X,Grid.X), FSnap(Y,Grid.Y), FSnap(Z,Grid.Z) );
	}
	FVector BoundToCube( FLOAT Radius )
	{
		return FVector
		(
			Clamp<FLOAT>( X, -Radius, Radius ),
			Clamp<FLOAT>( Y, -Radius, Radius ),
			Clamp<FLOAT>( Z, -Radius, Radius )
		);
	}
	void AddBounded( const FVector& V, FLOAT Radius=MAXSWORD )
	{
		*this = (*this+V).BoundToCube(Radius);
	}
	FLOAT& Component( INT Index )
	{
		return (&X)[Index];
	}

	// Return a boolean that is based on the vector's direction.
	// When      V==(0,0,0) Booleanize(0)=1.
	// Otherwise Booleanize(V) <-> !Booleanize(!B).
	UBOOL Booleanize()
	{
		return
			X >  0.f ? 1 :
			X <  0.f ? 0 :
			Y >  0.f ? 1 :
			Y <  0.f ? 0 :
			Z >= 0.f ? 1 : 0;
	}

	// Transformation.
	FVector TransformVectorBy( const FCoords& Coords ) const;
	FVector TransformPointBy( const FCoords& Coords ) const;
	FVector MirrorByVector( const FVector& MirrorNormal ) const;
	FVector MirrorByPlane( const FPlane& MirrorPlane ) const;

	// Complicated functions.
	CORE_API FRotator Rotation();
	CORE_API void FindBestAxisVectors( FVector& Axis1, FVector& Axis2 );
	CORE_API FVector SafeNormal() const; //warning: Not inline because of compiler bug.

	// Friends.
	friend FLOAT FDist( const FVector& V1, const FVector& V2 );
	friend FLOAT FDistSquared( const FVector& V1, const FVector& V2 );
	friend UBOOL FPointsAreSame( const FVector& P, const FVector& Q );
	friend UBOOL FPointsAreNear( const FVector& Point1, const FVector& Point2, FLOAT Dist);
	friend FLOAT FPointPlaneDist( const FVector& Point, const FVector& PlaneBase, const FVector& PlaneNormal );
	friend FVector FLinePlaneIntersection( const FVector& Point1, const FVector& Point2, const FVector& PlaneOrigin, const FVector& PlaneNormal );
	friend FVector FLinePlaneIntersection( const FVector& Point1, const FVector& Point2, const FPlane& Plane );
	friend UBOOL FParallel( const FVector& Normal1, const FVector& Normal2 );
	friend UBOOL FCoplanar( const FVector& Base1, const FVector& Normal1, const FVector& Base2, const FVector& Normal2 );

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FVector& V )
	{
		return Ar << V.X << V.Y << V.Z;
	}

	// Returns a string description.
	FString String() const
	{
		guardSlow(FVector::String);
		return FString::Printf( TEXT("(X=%f,Y=%f,Z=%f)"), X, Y, Z );
		unguardSlow;
	}
};

/*-----------------------------------------------------------------------------
	FPlane.
-----------------------------------------------------------------------------*/

class FPlane : public FVector
{
public:

	// Variables.
	union
	{
		struct {FLOAT W;};			// Coordinates.
		struct {FLOAT A;};			// Color components.
		struct {FLOAT Q;};			// The other thing.
	};

	// Constructors.
	FPlane()
	{}
	FPlane( const FPlane& P )
	:	FVector(P)
	,	W(P.W)
	{}
	FPlane( const FVector& V )
	:	FVector(V)
	,	W(0)
	{}
	FPlane( FLOAT InX, FLOAT InY, FLOAT InZ, FLOAT InW )
	:	FVector(InX,InY,InZ)
	,	W(InW)
	{}
	FPlane( FVector InNormal, FLOAT InW )
	:	FVector(InNormal), W(InW)
	{}
	FPlane( FVector InBase, const FVector &InNormal )
	:	FVector(InNormal)
	,	W(InBase | InNormal)
	{}
	FPlane( FVector A, FVector B, FVector C )
	:	FVector(((B-A)^(C-A)).SafeNormal())
	,	W(A | ((B-A)^(C-A)).SafeNormal())
	{}

	// Operators.
	UBOOL operator==( const FPlane& V ) const
	{
		return X==V.X && Y==V.Y && Z==V.Z && W==V.W;
	}
	UBOOL operator!=( const FPlane& V ) const
	{
		return X!=V.X || Y!=V.Y || Z!=V.Z || W!=V.W;
	}

	// Functions.
	FLOAT PlaneDot( const FVector& V ) const
	{
		return X*V.X+Y*V.Y+Z*V.Z-W;
	}
	FPlane Flip() const
	{
		return FPlane( -X, -Y, -Z, -W );
	}
	FPlane TransformPlaneByOrtho( const FCoords &Coords ) const;

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FPlane &P )
	{
		return Ar << (FVector&)P << P.W;
	}

	// Returns a string description.
	FString String() const
	{
		guardSlow(FPlane::String);
		return FString::Printf( TEXT("(X=%f,Y=%f,Z=%f,W=%f)"), X, Y, Z, W );
		unguardSlow;
	}
};

/*-----------------------------------------------------------------------------
	FSphere.
-----------------------------------------------------------------------------*/

class FSphere : public FPlane
{
public:

	// Constructors.
	FSphere()
	{}
	FSphere( INT )
	:	FPlane(0,0,0,0)
	{}
	FSphere( FVector V, FLOAT W )
	:	FPlane( V, W )
	{}
	FSphere( const FVector* Pts, INT Count );

	friend FArchive& operator<<( FArchive& Ar, FSphere& S )
	{
		guardSlow(FSphere<<);

		if( Ar.Ver()<=61 )//oldver
			Ar << (FVector&)S;
		else
			Ar << (FPlane&)S;

		return Ar;
		unguardSlow
	}

	// No new variables defined, so FPlane::String can be used.
};

/*-----------------------------------------------------------------------------
	FScale.
-----------------------------------------------------------------------------*/

// An axis along which sheering is performed.
enum ESheerAxis
{
	SHEER_None = 0x00,
	SHEER_XY   = 0x01,
	SHEER_XZ   = 0x02,
	SHEER_YX   = 0x03,
	SHEER_YZ   = 0x04,
	SHEER_ZX   = 0x05,
	SHEER_ZY   = 0x06,

	SHEER_MAX  = 0xFF
};

inline FString appSheerAxisString( const BYTE SheerAxis )
{
	guard(appSheerAxisString);
	switch ( SheerAxis )
	{
		case SHEER_None: return TEXT("None");
		case SHEER_XY:   return TEXT("XY");
		case SHEER_XZ:   return TEXT("XZ");
		case SHEER_YX:   return TEXT("YX");
		case SHEER_YZ:   return TEXT("YZ");
		case SHEER_ZX:   return TEXT("ZX");
		case SHEER_ZY:   return TEXT("ZY");

		// MAX.
		case SHEER_MAX:  return TEXT("MAX");

		// Unknown.
		default:         return FString::Printf( TEXT("0x%02X"), SheerAxis );
	}
	unguard;
}

//
// Scaling and sheering info associated with a brush.  This is 
// easily-manipulated information which is built into a transformation
// matrix later.
//
class FScale 
{
public:
	// Variables.
	FVector Scale;
	FLOAT   SheerRate;
	BYTE    SheerAxis; // ESheerAxis.

	// Constructors.
	FScale()
	{}
	FScale( const FVector &InScale, FLOAT InSheerRate, ESheerAxis InSheerAxis )
	:	Scale(InScale), SheerRate(InSheerRate), SheerAxis(InSheerAxis)
	{}

	// Operators.
	UBOOL operator==( const FScale &S ) const
	{
		return Scale==S.Scale && SheerRate==S.SheerRate && SheerAxis==S.SheerAxis;
	}

	// Functions.
	FLOAT Orientation()
	{
		return Sgn( Scale.X*Scale.Y*Scale.Z );
	}

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FScale &S )
	{
		return Ar << S.Scale << S.SheerRate << S.SheerAxis;
	}

	// Returns a string description.
	FString String() const
	{
		guardSlow(FScale::String);
		return FString::Printf( TEXT("(Scale=%s,SheerRate=%f,SheerAxis=%s)"), *Scale.String(), SheerRate, *appSheerAxisString(SheerAxis) );
		unguardSlow;
	}
};

/*-----------------------------------------------------------------------------
	FCoords.
-----------------------------------------------------------------------------*/

//
// A coordinate system matrix.
//
class FCoords
{
public:
	FVector	Origin;
	FVector	XAxis;
	FVector YAxis;
	FVector ZAxis;

	// Constructors.
	FCoords()
	{}
	FCoords( const FVector &InOrigin )
	:	Origin(InOrigin)
	, XAxis(1.0f,0.0f,0.0f)
	, YAxis(0.0f,1.0f,0.0f)
	, ZAxis(0.0f,0.0f,1.0f)
	{}
	FCoords( const FVector &InOrigin, const FVector &InX, const FVector &InY, const FVector &InZ )
	:	Origin(InOrigin)
	, XAxis(InX)
	, YAxis(InY)
	, ZAxis(InZ)
	{}

	// Functions.
	FCoords MirrorByVector( const FVector& MirrorNormal ) const;
	FCoords MirrorByPlane( const FPlane& MirrorPlane ) const;
	FCoords Transpose() const;
	CORE_API FCoords Inverse() const;
	//FCoords PivotInverse() const; // Inverse() but with order of Translation and Rotation flipped.
	//FCoords ApplyPivot( const FCoords& CoordsB ) const;
	CORE_API FRotator OrthoRotation() const;

	// Operators.
	FCoords& operator*=	(const FCoords   &TransformCoords);
	FCoords	 operator*	(const FCoords   &TransformCoords) const;
	FCoords& operator*=	(const FVector   &Point);
	FCoords  operator*	(const FVector   &Point) const;
	FCoords& operator*=	(const FRotator  &Rot);
	FCoords  operator*	(const FRotator  &Rot) const;
	FCoords& operator*=	(const FScale    &Scale);
	FCoords  operator*	(const FScale    &Scale) const;
	FCoords& operator/=	(const FVector   &Point);
	FCoords  operator/	(const FVector   &Point) const;
	FCoords& operator/=	(const FRotator  &Rot);
	FCoords  operator/	(const FRotator  &Rot) const;
	FCoords& operator/=	(const FScale    &Scale);
	FCoords  operator/	(const FScale    &Scale) const;

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FCoords& C )
	{
		return Ar << C.Origin << C.XAxis << C.YAxis << C.ZAxis;
	}

	FString String() const
	{
		guardSlow(FCoords::String);
		return FString::Printf( TEXT("(Origin=%s,XAxis=%s,YAxis=%s,ZAxis=%s)"), *Origin.String(), *XAxis.String(), *YAxis.String(), *ZAxis.String() );
		unguardSlow;
	}
};

/*-----------------------------------------------------------------------------
	FModelCoords.
-----------------------------------------------------------------------------*/

//
// A model coordinate system, describing both the covariant and contravariant
// transformation matrices to transform points and normals by.
//
class FModelCoords
{
public:
	// Variables.
	FCoords PointXform;  // Coordinates to transform points by  (covariant).
	FCoords VectorXform; // Coordinates to transform normals by (contravariant).

	// Constructors.
	FModelCoords()
	{}
	FModelCoords( const FCoords& InCovariant, const FCoords& InContravariant )
	:	PointXform(InCovariant)
	, VectorXform(InContravariant)
	{}

	// Functions.
	FModelCoords Inverse()
	{
		return FModelCoords( VectorXform.Transpose(), PointXform.Transpose() );
	}

	// Returns a string description.
	FString String() const
	{
		guardSlow(FModelCoords::String);
		return FString::Printf( TEXT("(PointXform=%s,VectorXform=%s)"), *PointXform.String(), *VectorXform.String() );
		unguardSlow;
	}
};

/*-----------------------------------------------------------------------------
	FRotator.
-----------------------------------------------------------------------------*/

//
// Rotation.
//
class FRotator
{
public:
	// Variables.
	INT Pitch; // Looking up and down (0=Straight Ahead, +Up, -Down).
	INT Yaw;   // Rotating around (running in circles), 0=East, +North, -South.
	INT Roll;  // Rotation about axis of screen, 0=Straight, +Clockwise, -CCW.

	// Constructors.
	FRotator()
	{}
	FRotator( INT InPitch, INT InYaw, INT InRoll )
	:	Pitch(InPitch)
	, Yaw(InYaw)
	, Roll(InRoll)
	{}

	// Binary arithmetic operators.
	FRotator operator+( const FRotator &R ) const
	{
		return FRotator( Pitch+R.Pitch, Yaw+R.Yaw, Roll+R.Roll );
	}
	FRotator operator-( const FRotator &R ) const
	{
		return FRotator( Pitch-R.Pitch, Yaw-R.Yaw, Roll-R.Roll );
	}
	FRotator operator*( FLOAT Scale ) const
	{
		return FRotator( appRound(Pitch*Scale), appRound(Yaw*Scale), appRound(Roll*Scale) );
	}
	friend FRotator operator*( FLOAT Scale, const FRotator &R )
	{
		return FRotator( appRound(R.Pitch*Scale), appRound(R.Yaw*Scale), appRound(R.Roll*Scale) );
	}
	FRotator operator*= (FLOAT Scale)
	{
		Pitch = appRound(Pitch*Scale); Yaw = appRound(Yaw*Scale); Roll = appRound(Roll*Scale);
		return *this;
	}
	// Binary comparison operators.
	UBOOL operator==( const FRotator &R ) const
	{
		return Pitch==R.Pitch && Yaw==R.Yaw && Roll==R.Roll;
	}
	UBOOL operator!=( const FRotator &V ) const
	{
		return Pitch!=V.Pitch || Yaw!=V.Yaw || Roll!=V.Roll;
	}
	// Assignment operators.
	FRotator operator+=( const FRotator &R )
	{
		Pitch += R.Pitch; Yaw += R.Yaw; Roll += R.Roll;
		return *this;
	}
	FRotator operator-=( const FRotator &R )
	{
		Pitch -= R.Pitch; Yaw -= R.Yaw; Roll -= R.Roll;
		return *this;
	}
	// Functions.
	FRotator Reduce() const
	{
		return FRotator( ReduceAngle(Pitch), ReduceAngle(Yaw), ReduceAngle(Roll) );
	}
	int IsZero() const
	{
		return ((Pitch|Yaw|Roll)&65535)==0);
	}
	FRotator Add( INT DeltaPitch, INT DeltaYaw, INT DeltaRoll )
	{
		Yaw   += DeltaYaw;
		Pitch += DeltaPitch;
		Roll  += DeltaRoll;

		return *this;
	}
	FRotator AddBounded( INT DeltaPitch, INT DeltaYaw, INT DeltaRoll )
	{
		Yaw  += DeltaYaw;

		Pitch = FAddAngleConfined( Pitch,DeltaPitch, 192*256, 64*256 );
		Roll  = FAddAngleConfined( Roll, DeltaRoll,  192*256, 64*256 );

		return *this;
	}
	FRotator GridSnap( const FRotator &RotGrid )
	{
		return FRotator
		(
			FSnap( Pitch, RotGrid.Pitch ),
			FSnap( Yaw,   RotGrid.Yaw   ),
			FSnap( Roll,  RotGrid.Roll  )
		);
	}
	FVector Vector();

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FRotator& R )
	{
		return Ar << R.Pitch << R.Yaw << R.Roll;
	}

	// Returns a string description.
	FString String() const
	{
		guardSlow(FRotator::String);
		return FString::Printf( TEXT("(Pitch=%i,Yaw=%i,Roll=%i)"), Pitch, Yaw, Roll );
		unguardSlow;
	}
};

/*-----------------------------------------------------------------------------
	Bounds.
-----------------------------------------------------------------------------*/

//
// A rectangular minimum bounding volume.
//
class FBox
{
public:
	// Variables.
	FVector Min;
	FVector Max;
	BYTE IsValid;

	// Constructors.
	FBox()
	{}
	FBox( INT )
	: Min(0,0,0)
	, Max(0,0,0)
	, IsValid(0)
	{}
	FBox( const FVector& InMin, const FVector& InMax )
	: Min(InMin)
	, Max(InMax)
	, IsValid(1)
	{}
	CORE_API FBox( const FVector* Points, INT Count );

	// Accessors.
	FVector& GetExtrema( int i )
	{
		return (&Min)[i];
	}
	const FVector& GetExtrema( int i ) const
	{
		return (&Min)[i];
	}

	// Functions.
	FBox& operator+=( const FVector &Other )
	{
		if( IsValid )
		{
			Min.X = ::Min<FLOAT>( Min.X, Other.X );
			Min.Y = ::Min<FLOAT>( Min.Y, Other.Y );
			Min.Z = ::Min<FLOAT>( Min.Z, Other.Z );

			Max.X = ::Max<FLOAT>( Max.X, Other.X );
			Max.Y = ::Max<FLOAT>( Max.Y, Other.Y );
			Max.Z = ::Max<FLOAT>( Max.Z, Other.Z );
		}
		else
		{
			Min = Max = Other;
			IsValid = 1;
		}
		return *this;
	}
	FBox operator+( const FVector& Other ) const
	{
		return FBox(*this) += Other;
	}
	FBox& operator+=( const FBox& Other )
	{
		if( IsValid && Other.IsValid )
		{
			Min.X = ::Min<FLOAT>( Min.X, Other.Min.X );
			Min.Y = ::Min<FLOAT>( Min.Y, Other.Min.Y );
			Min.Z = ::Min<FLOAT>( Min.Z, Other.Min.Z );
                 
			Max.X = ::Max<FLOAT>( Max.X, Other.Max.X );
			Max.Y = ::Max<FLOAT>( Max.Y, Other.Max.Y );
			Max.Z = ::Max<FLOAT>( Max.Z, Other.Max.Z );
		}             
		else
		{
			*this = Other;
		}
		return *this;
	}
	FBox operator+( const FBox& Other ) const
	{
		return FBox(*this) += Other;
	}
	FBox TransformBy( const FCoords& Coords ) const
	{
		FBox NewBox(0);
		for ( INT i=0; i<2; i++ )
			for ( INT j=0; j<2; j++ )
				for ( INT k=0; k<2; k++ )
					NewBox += FVector( GetExtrema(i).X, GetExtrema(j).Y, GetExtrema(k).Z ).TransformPointBy( Coords );
		return NewBox;
	}
	FBox ExpandBy( FLOAT W ) const
	{
		return FBox( Min-FVector(W,W,W), Max+FVector(W,W,W) );
	}

	// Serializer.
	friend FArchive& operator<<( FArchive& Ar, FBox& Bound )
	{
		return Ar << Bound.Min << Bound.Max << Bound.IsValid;
	}

	// Returns a string description.
	FString String() const
	{
		guardSlow(FBox::String);
		return FString::Printf( TEXT("(Min=%s,Max=%s,IsValid=%i)"), *Min.String(), *Max.String(), IsValid );
		unguardSlow;
	}
};

/*-----------------------------------------------------------------------------
	FGlobalMath.
-----------------------------------------------------------------------------*/

//
// Global mathematics info.
//
class FGlobalMath
{
public:
	// Constants.
	enum { ANGLE_SHIFT = 2     }; // Bits to right-shift to get lookup value.
	enum { ANGLE_BITS  = 14    }; // Number of valid bits in angles.
	enum { NUM_ANGLES  = 16384 }; // Number of angles that are in lookup table.
	enum { NUM_SQRTS   = 16384 }; // Number of square roots in lookup table.
	enum { ANGLE_MASK  = (((1<<ANGLE_BITS)-1)<<(16-ANGLE_BITS)) };

	// Class constants.
	const FVector WorldMin;
	const FVector WorldMax;
	const FCoords UnitCoords;
	const FScale  UnitScale;
	const FCoords ViewCoords;

	// Constructor.
	CORE_API FGlobalMath();

	// Basic math functions.
	FLOAT Sqrt( INT i )
	{
		return SqrtFLOAT[i]; 
	}
	FLOAT SinTab( INT i )
	{
		return TrigFLOAT[((i>>ANGLE_SHIFT)&(NUM_ANGLES-1))];
	}
	FLOAT CosTab( INT i )
	{
		return TrigFLOAT[(((i+16384)>>ANGLE_SHIFT)&(NUM_ANGLES-1))];
	}
	FLOAT SinFloat( FLOAT F )
	{
		return SinTab(appRound((F*65536.f)/(2.f*PI)));
	}
	FLOAT CosFloat( FLOAT F )
	{
		return CosTab(appRound((F*65536.f)/(2.f*PI)));
	}

private:

	// Tables.
	FLOAT TrigFLOAT[NUM_ANGLES];
	FLOAT SqrtFLOAT[NUM_SQRTS];
	FLOAT LightSqrtFLOAT[NUM_SQRTS];
};

inline INT ReduceAngle( INT Angle )
{
	return Angle & FGlobalMath::ANGLE_MASK;
};

/*-----------------------------------------------------------------------------
	Floating point constants.
-----------------------------------------------------------------------------*/

//
// Lengths of normalized vectors (These are half their maximum values
// to assure that dot products with normalized vectors don't overflow).
//
#define FLOAT_NORMAL_THRESH          (0.0001f)

//
// Magic numbers for numerical precision.
//
#define THRESH_POINT_ON_PLANE          (0.10f) /* Thickness of plane for front/back/inside test */
#define THRESH_POINT_ON_SIDE           (0.20f) /* Thickness of polygon side's side-plane for point-inside/outside/on side test */
#define THRESH_POINTS_ARE_SAME        (0.002f) /* Two points are same if within this distance */
#define THRESH_POINTS_ARE_NEAR        (0.015f) /* Two points are near if within this distance and can be combined if imprecise math is ok */
#define THRESH_NORMALS_ARE_SAME     (0.00002f) /* Two normal points are same if within this distance */
													/* Making this too large results in incorrect CSG classification and disaster */
#define THRESH_VECTORS_ARE_NEAR      (0.0004f) /* Two vectors are near if within this distance and can be combined if imprecise math is ok */
													/* Making this too large results in lighting problems due to inaccurate texture coordinates */
#define THRESH_SPLIT_POLY_WITH_PLANE   (0.25f) /* A plane splits a polygon in half */
#define THRESH_SPLIT_POLY_PRECISELY    (0.01f) /* A plane exactly splits a polygon */
#define THRESH_ZERO_NORM_SQUARED     (0.0001f) /* Size of a unit normal that is considered "zero", squared */
#define THRESH_VECTORS_ARE_PARALLEL    (0.02f) /* Vectors are parallel if dot product varies less than this */

/*-----------------------------------------------------------------------------
	FVector transformation.
-----------------------------------------------------------------------------*/

//
// Transform a point by a coordinate system, moving
// it by the coordinate system's origin if nonzero.
//
inline FVector FVector::TransformPointBy( const FCoords &Coords ) const
{
	FVector Temp = *this-Coords.Origin;
	return FVector(	Temp|Coords.XAxis, Temp|Coords.YAxis, Temp|Coords.ZAxis );
}

//
// Transform a directional vector by a coordinate system.
// Ignore's the coordinate system's origin.
//
inline FVector FVector::TransformVectorBy( const FCoords &Coords ) const
{
	return FVector(	*this|Coords.XAxis, *this|Coords.YAxis, *this|Coords.ZAxis );
}

//
// Mirror a vector about a normal vector.
//
// Also called "reflect".
//
inline FVector FVector::MirrorByVector( const FVector& MirrorNormal ) const
{
	return *this-MirrorNormal*(2.0f*(*this|MirrorNormal));
}

//
// Mirror a vector about a plane.
//
inline FVector FVector::MirrorByPlane( const FPlane& Plane ) const
{
	return *this-Plane*(2.0f*Plane.PlaneDot(*this));
}

/*-----------------------------------------------------------------------------
	FVector friends.
-----------------------------------------------------------------------------*/

//
// Compare two points and see if they're the same, using a threshold.
// Returns 1=yes, 0=no.  Uses fast distance approximation.
//
inline UBOOL FPointsAreNear( const FVector& P, const FVector& Q, FLOAT Dist )
{
	FLOAT Temp;
	if (Abs<FLOAT>(P.X-Q.X)>=Dist) return 0;
	if (Abs<FLOAT>(P.Y-Q.Y)>=Dist) return 0;
	if (Abs<FLOAT>(P.Z-Q.Z)>=Dist) return 0;
	return 1;
}

//
// Compare two points and see if they're the same, using a threshold.
// Returns 1=yes, 0=no.  Uses fast distance approximation.
//
inline UBOOL FPointsAreSame( const FVector& P, const FVector& Q )
{
	return FPointsAreNear( P, Q, THRESH_POINTS_ARE_SAME )
}

//
// Calculate the signed distance (in the direction of the normal) between
// a point and a plane.
//
inline FLOAT FPointPlaneDist( const FVector &Point, const FVector &PlaneBase, const FVector &PlaneNormal )
{
	return (Point-PlaneBase) | PlaneNormal;
}

//
// Euclidean distance between two points.
//
inline FLOAT FDist( const FVector &V1, const FVector &V2 )
{
	return (V2-V1).Size();
}

//
// Squared distance between two points.
//
inline FLOAT FDistSquared( const FVector &V1, const FVector &V2 )
{
	return (V2-V1).SizeSquared();
}

//
// Euclidean distance between two points in xy plane.
//
inline FLOAT FDist2D( const FVector &V1, const FVector &V2 )
{
	return (V2-V1).Size2D();
}

//
// Squared distance between two points in xy plane.
//
inline FLOAT FDistSquared2D( const FVector &V1, const FVector &V2 )
{
	return (V2-V1).SizeSquared2D();
}

//
// See if two normal vectors (or plane normals) are nearly parallel.
//
inline UBOOL FParallel( const FVector &Normal1, const FVector &Normal2 )
{
	// The double abs is intended for case where the dot product is slightly larger than 1.
	return Abs<FLOAT>(Abs<FLOAT>(Normal1|Normal2)-1.f)<=THRESH_VECTORS_ARE_PARALLEL;
}

//
// See if two planes are coplanar.
//
inline int FCoplanar( const FVector &Base1, const FVector &Normal1, const FVector &Base2, const FVector &Normal2 )
{
	return FParallel(Normal1,Normal2) && FPointPlaneDist(Base2,Base1,Normal1)<=THRESH_POINT_ON_PLANE;
}

//
// Triple product of three vectors.
//
inline FLOAT FTriple( const FVector& X, const FVector& Y, const FVector& Z )
{
	return X|(Y^Z);
}

//
// Volume spanned by three vectors.
//
inline FLOAT FVolume( const FVector& X, const FVector& Y, const FVector& Z )
{
	return Abs<FLOAT>(FTriple);
}

/*-----------------------------------------------------------------------------
	FPlane implementation.
-----------------------------------------------------------------------------*/

//
// Transform a point by a coordinate system, moving
// it by the coordinate system's origin if nonzero.
//
inline FPlane FPlane::TransformPlaneByOrtho( const FCoords &Coords ) const
{
	FVector Normal( *this|Coords.XAxis, *this|Coords.YAxis, *this|Coords.ZAxis );
	return FPlane( Normal, W-(Coords.Origin.TransformVectorBy(Coords)|Normal) );
}

/*-----------------------------------------------------------------------------
	FCoords functions.
-----------------------------------------------------------------------------*/

//
// Return this coordinate system's transpose.
// If the coordinate system is orthogonal, this is equivalent to its inverse.
//
inline FCoords FCoords::Transpose() const
{
	return FCoords
	(
		-Origin.TransformVectorBy(*this),
		FVector( XAxis.X, YAxis.X, ZAxis.X ),
		FVector( XAxis.Y, YAxis.Y, ZAxis.Y ),
		FVector( XAxis.Z, YAxis.Z, ZAxis.Z )
	);
}

//
// Mirror the coordinates about a normal vector.
//
inline FCoords FCoords::MirrorByVector( const FVector& MirrorNormal ) const
{
	return FCoords
	(
		Origin.MirrorByVector( MirrorNormal ),
		XAxis.MirrorByVector( MirrorNormal ),
		YAxis.MirrorByVector( MirrorNormal ),
		ZAxis.MirrorByVector( MirrorNormal )
	);
}

//
// Mirror the coordinates about a plane.
//
inline FCoords FCoords::MirrorByPlane( const FPlane& Plane ) const
{
	return FCoords
	(
		Origin.MirrorByPlane( Plane ),
		XAxis.MirrorByVector( Plane ),
		YAxis.MirrorByVector( Plane ),
		ZAxis.MirrorByVector( Plane )
	);
}

/*-----------------------------------------------------------------------------
	FCoords operators.
-----------------------------------------------------------------------------*/

//
// Transform this coordinate system by another coordinate system.
//
inline FCoords& FCoords::operator*=( const FCoords& TransformCoords )
{
	//!! Proper solution:
	//Origin = Origin.TransformPointBy( TransformCoords.Inverse().Transpose() );
	// Fast solution assuming orthogonal coordinate system:
	Origin = Origin.TransformPointBy( TransformCoords );
	XAxis  = XAxis.TransformVectorBy( TransformCoords );
	YAxis  = YAxis.TransformVectorBy( TransformCoords );
	ZAxis  = ZAxis.TransformVectorBy( TransformCoords );

	return *this;
}
inline FCoords FCoords::operator*( const FCoords& TransformCoords ) const
{
	return FCoords(*this) *= TransformCoords;
}

//
// Transform this coordinate system by a pitch-yaw-roll rotation.
//
inline FCoords& FCoords::operator*=( const FRotator& Rot )
{
	// Apply yaw rotation.
	*this *= FCoords
	(	
		FVector( +0.f,                   +0.f,                   +0.f ),
		FVector( +GMath.CosTab(Rot.Yaw), +GMath.SinTab(Rot.Yaw), +0.f ),
		FVector( -GMath.SinTab(Rot.Yaw), +GMath.CosTab(Rot.Yaw), +0.f ),
		FVector( +0.f,                   +0.f,                   +1.f )
	);

	// Apply pitch rotation.
	*this *= FCoords
	(	
		FVector( +0.f,                     +0.f, +0.f                     ),
		FVector( +GMath.CosTab(Rot.Pitch), +0.f, +GMath.SinTab(Rot.Pitch) ),
		FVector( +0.f,                     +1.f, +0.f                     ),
		FVector( -GMath.SinTab(Rot.Pitch), +0.f, +GMath.CosTab(Rot.Pitch) )
	);

	// Apply roll rotation.
	*this *= FCoords
	(	
		FVector( +0.f, +0.f,                    0.0f                    ),
		FVector( +1.f, +0.f,                    +0.f                    ),
		FVector( +0.f, +GMath.CosTab(Rot.Roll), -GMath.SinTab(Rot.Roll) ),
		FVector( +0.f, +GMath.SinTab(Rot.Roll), +GMath.CosTab(Rot.Roll) )
	);
	return *this;
}
inline FCoords FCoords::operator*( const FRotator& Rot ) const
{
	return FCoords(*this) *= Rot;
}

inline FCoords& FCoords::operator*=( const FVector& Point )
{
	Origin -= Point;
	return *this;
}
inline FCoords FCoords::operator*( const FVector& Point ) const
{
	return FCoords(*this) *= Point;
}

//
// Detransform this coordinate system by a pitch-yaw-roll rotation.
//
inline FCoords& FCoords::operator/=( const FRotator& Rot )
{
	// Apply inverse roll rotation.
	*this *= FCoords
	(
		FVector( 0.f, 0.f, 0.f ),
		FVector( +1.f, -0.f, +0.f ),
		FVector( -0.f, +GMath.CosTab(Rot.Roll), +GMath.SinTab(Rot.Roll) ),
		FVector( +0.f, -GMath.SinTab(Rot.Roll), +GMath.CosTab(Rot.Roll) )
	);

	// Apply inverse pitch rotation.
	*this *= FCoords
	(
		FVector( 0.f, 0.f, 0.f ),
		FVector( +GMath.CosTab(Rot.Pitch), +0.f, -GMath.SinTab(Rot.Pitch) ),
		FVector( +0.f, +1.f, -0.f ),
		FVector( +GMath.SinTab(Rot.Pitch), +0.f, +GMath.CosTab(Rot.Pitch) )
	);

	// Apply inverse yaw rotation.
	*this *= FCoords
	(
		FVector( 0.f, 0.f, 0.f ),
		FVector( +GMath.CosTab(Rot.Yaw), -GMath.SinTab(Rot.Yaw), -0.f ),
		FVector( +GMath.SinTab(Rot.Yaw), +GMath.CosTab(Rot.Yaw), +0.f ),
		FVector( -0.f, +0.f, +1.f )
	);
	return *this;
}
inline FCoords FCoords::operator/( const FRotator& Rot ) const
{
	return FCoords(*this) /= Rot;
}

inline FCoords& FCoords::operator/=( const FVector& Point )
{
	Origin += Point;
	return *this;
}
inline FCoords FCoords::operator/( const FVector& Point ) const
{
	return FCoords(*this) /= Point;
}

//
// Transform this coordinate system by a scale.
// Note: Will return coordinate system of opposite handedness if
// Scale.X*Scale.Y*Scale.Z is negative.
//
inline FCoords& FCoords::operator*=( const FScale& Scale )
{
	// Apply sheering.
	FLOAT   Sheer      = FSheerSnap( Scale.SheerRate );
	FCoords TempCoords = GMath.UnitCoords;

	switch( Scale.SheerAxis )
	{
		case SHEER_XY:
			TempCoords.XAxis.Y = Sheer;
			break;
		case SHEER_XZ:
			TempCoords.XAxis.Z = Sheer;
			break;
		case SHEER_YX:
			TempCoords.YAxis.X = Sheer;
			break;
		case SHEER_YZ:
			TempCoords.YAxis.Z = Sheer;
			break;
		case SHEER_ZX:
			TempCoords.ZAxis.X = Sheer;
			break;
		case SHEER_ZY:
			TempCoords.ZAxis.Y = Sheer;
			break;
		default:
			break;
	}
	*this *= TempCoords;

	// Apply scaling.
	XAxis    *= Scale.Scale;
	YAxis    *= Scale.Scale;
	ZAxis    *= Scale.Scale;
	Origin.X /= Scale.Scale.X;
	Origin.Y /= Scale.Scale.Y;
	Origin.Z /= Scale.Scale.Z;

	return *this;
}
inline FCoords FCoords::operator*( const FScale& Scale ) const
{
	return FCoords(*this) *= Scale;
}

//
// Detransform a coordinate system by a scale.
//
inline FCoords& FCoords::operator/=( const FScale& Scale )
{
	// Deapply scaling.
	XAxis    /= Scale.Scale;
	YAxis    /= Scale.Scale;
	ZAxis    /= Scale.Scale;
	Origin.X *= Scale.Scale.X;
	Origin.Y *= Scale.Scale.Y;
	Origin.Z *= Scale.Scale.Z;

	// Deapply sheering.
	FCoords TempCoords(GMath.UnitCoords);
	FLOAT Sheer = FSheerSnap( Scale.SheerRate );
	switch( Scale.SheerAxis )
	{
		case SHEER_XY:
			TempCoords.XAxis.Y = -Sheer;
			break;
		case SHEER_XZ:
			TempCoords.XAxis.Z = -Sheer;
			break;
		case SHEER_YX:
			TempCoords.YAxis.X = -Sheer;
			break;
		case SHEER_YZ:
			TempCoords.YAxis.Z = -Sheer;
			break;
		case SHEER_ZX:
			TempCoords.ZAxis.X = -Sheer;
			break;
		case SHEER_ZY:
			TempCoords.ZAxis.Y = -Sheer;
			break;
		default: // SHEER_NONE
			break;
	}
	*this *= TempCoords;

	return *this;
}
inline FCoords FCoords::operator/( const FScale& Scale ) const
{
	return FCoords(*this) /= Scale;
}

/*-----------------------------------------------------------------------------
	Random numbers.
-----------------------------------------------------------------------------*/

//
// Compute pushout of a box from a plane.
//
inline FLOAT FBoxPushOut( FVector Normal, FVector Size )
{
	return Abs<FLOAT>(Normal.X*Size.X) + Abs<FLOAT>(Normal.Y*Size.Y) + Abs<FLOAT>(Normal.Z*Size.Z);
}

//
// Return a uniformly distributed random unit vector.
//
inline FVector VRand()
{
	FVector Result;
	do
	{
		// Check random vectors in the unit sphere so result is statistically uniform.
		Result.X = appFrand()*2.f-1.f;
		Result.Y = appFrand()*2.f-1.f;
		Result.Z = appFrand()*2.f-1.f;
	} while( Result.SizeSquared()>1.f );

	return Result.UnsafeNormal();
}

//
// RandomSpreadVector
// By Paul Du Bois, Infinite Machine
//
// Return a random unit vector within a cone of SpreadDegrees.
// (Distribution is such that there is no bunching near the axis.)
//
// - Create an FRotator with Pitch < SpreadDegrees/2, Yaw = 0, Roll = random.
//   The tricky bit is getting the probability distribution of pitch
//   correct.  If it's flat, particles will tend to cluster around the pole.
//
// - For a given pitch phi, the probability p(phi) should be proportional to
//   the surface area of the slice of sphere defined by that pitch and a
//   random roll.  This is 2 pi (r sin phi) (r d phi) =~ sin phi
//
// - To map a flat distribution (FRand) to our new p(phi), we first find the
//   CDF (cumulative distribution fn) which is basically the integral of
//   p(phi) over its domain, normalized to have a total area of 1.  Smaller
//   spreads are modeled by limiting the domain of p(phi) and normalizing
//   the CDF for the smaller domain.  The CDF is always monotonically
//   non-decreasing and has range [0,1].
//
// - The insight is that the inverse of the CDF is exactly what we need to
//   convert a flat distribution to our p(phi).  aCDF: [0,1] -> angle.
//   (insert handwaving argument)
//
// - CDF(x) = integral(0->x) sin phi d phi
//          = -cos(x) - -cos(0)
//          = K * (1 - cos(x)) (K is normalizing factor to make CDF(domain_max)=1)
//        K = 1/max_cos_val   max_cos_val = (1 - cos(max pitch))
//
// - aCDF(y) = acos(1-y/K) = acos(1-y*max_cos_val)
//
inline FVector RandomSpreadVector( FLOAT SpreadDegrees )
{
	FLOAT MaxPitch = Clamp<FLOAT>( SpreadDegrees*(PI/180.0f/2.0f), 0.0f , 180.0f );
	FLOAT K        = 1.0f-appCos(MaxPitch);
	FLOAT Pitch    = appAcos( 1.0f-appFrand()*K ); // This is the aCDF.
	FLOAT RandRoll = appFrand()*2.0f*PI;
	FLOAT Radius   = appSin(Pitch);

	return FVector
	(
		appCos(Pitch),
		Radius*appSin(RandRoll),
		Radius*appCos(RandRoll)
	);
}

/*-----------------------------------------------------------------------------
	Advanced geometry.
-----------------------------------------------------------------------------*/

//
// Find the intersection of an infinite line (defined by two points) and
// a plane.  Assumes that the line and plane do indeed intersect; you must
// make sure they're not parallel before calling.
//
inline FVector FLinePlaneIntersection( const FVector& P, const FVector& Q, const FVector& PlaneOrigin, const FVector& PlaneNormal )
{
	return P+(Q-P)*(((PlaneOrigin-P)|PlaneNormal)/((Q-P)|PlaneNormal));
}
inline FVector FLinePlaneIntersection( const FVector& P, const FVector& Q, const FPlane& Plane )
{
	return P+(Q-P)*((Plane.W-(P|Plane))/((Q-P)|Plane));
}

/*-----------------------------------------------------------------------------
	FPlane functions.
-----------------------------------------------------------------------------*/

//
// Compute intersection point of three planes.
// Return 1 if valid, 0 if infinite.
// Will set Intersection to (0,0,0) when not valid.
//
inline UBOOL FIntersectPlanes3( FVector& Intersection, const FPlane& P1, const FPlane& P2, const FPlane& P3 )
{
	guard(FIntersectPlanes3);

	// Compute determinant, the triple product P1|(P2^P3)==(P1^P2)|P3.
	FLOAT Det = (P1^P2)|P3;
	if ( Square<FLOAT>(Det)<Square<FLOAT>(0.001f) )
	{
		// Degenerate.
		Intersection = FVector(0,0,0);
		return 0;
	}
	else
	{
		// Compute the intersection point, guaranteed valid if determinant is nonzero.
		Intersection = (P1.W*(P2^P3)+P2.W*(P3^P1)+P3.W*(P1^P2))/Det;
	}
	return 1;
	unguard;
}

//
// Compute intersection point and direction of line joining two planes.
// Return 1 if valid, 0 if infinite.
//
inline UBOOL FIntersectPlanes2( FVector& I, FVector& D, const FPlane& P1, const FPlane& P2 )
{
	guard(FIntersectPlanes2);

	// Compute line direction, perpendicular to both plane normals.
	D = P1 ^ P2;
	FLOAT DD = D.SizeSquared();
	if ( DD<Square<FLOAT>(0.001f) )
	{
		// Parallel or nearly parallel planes.
		D = I = FVector(0,0,0);
		return 0;
	}
	else
	{
		// Compute intersection.
		I = (P1.W*(P2^D) + P2.W*(D^P1)) / DD;
		D.Normalize();
		return 1;
	}
	unguard;
}

/*-----------------------------------------------------------------------------
	FRotator functions.
-----------------------------------------------------------------------------*/

//
// Convert a rotation into a vector facing in its direction.
//
inline FVector FRotator::Vector()
{
	return (GMath.UnitCoords/(*this)).XAxis;
}


/*-----------------------------------------------------------------------------
	Fast 32-bit float evaluations. 

	Warning: likely not portable, and useful on Pentium class processors only.
-----------------------------------------------------------------------------*/

#if !_REALLY_WANT_FP32_HACKS // You really should not be using these anymore --han
inline UBOOL IsSmallerPositiveFloat( FLOAT F1, FLOAT F2 )
{
	return (*(DWORD*)&F1)<(*(DWORD*)&F2);
}
inline FLOAT MinPositiveFloat( FLOAT F1, FLOAT F2 )
{
	if ( IsSmallerPositiveFloat(F1,F2) )
		return F1;
	else
		return F2;
}

//
// Warning: 0 and -0 have different binary representations.
//
inline UBOOL EqualPositiveFloat(float F1, float F2)
{
	return ( *(DWORD*)&F1 == *(DWORD*)&F2 );
}

inline UBOOL IsNegativeFloat(float F1)
{
	return ( (*(DWORD*)&F1) >= (DWORD)0x80000000 ); // Detects sign bit.
}

inline FLOAT MaxPositiveFloat(float F1, float F2)
{
	if ( (*(DWORD*)&F1) < (*(DWORD*)&F2)) return F2; else return F1;
}

// Clamp F0 between F1 and F2, all positive assumed.
inline FLOAT ClampPositiveFloat(float F0, float F1, float F2)
{
	if      ( (*(DWORD*)&F0) < (*(DWORD*)&F1)) return F1;
	else if ( (*(DWORD*)&F0) > (*(DWORD*)&F2)) return F2;
	else return F0;
}

// Clamp any float F0 between zero and positive float Range
#define ClipFloatFromZero(F0,Range)\
{\
	if ( (*(DWORD*)&F0) >= (DWORD)0x80000000) F0 = 0.f;\
	else if	( (*(DWORD*)&F0) > (*(DWORD*)&Range)) F0 = Range;\
}

#endif //_REALLY_WANT_FP32_HACKS.

/*-----------------------------------------------------------------------------
	The End.
-----------------------------------------------------------------------------*/
