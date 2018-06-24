//
//  gvector3float.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gvector3float.h"
#include "gmath.h"
#include "gassert.h"
	
//static public members
/*static*/ const GVector3Float GVector3Float::sZero(0.0F, 0.0F, 0.0F);
/*static*/ const GVector3Float GVector3Float::sUnit(1.0F, 1.0F, 1.0F);
/*static*/ const GVector3Float GVector3Float::sUnitX(1.0F, 0.0F, 0.0F);
/*static*/ const GVector3Float GVector3Float::sUnitY(0.0F, 1.0F, 0.0F);
/*static*/ const GVector3Float GVector3Float::sUnitZ(0.0F, 0.0F, 1.0F);

//constructors
GVector3Float::GVector3Float(const GR32 in_dataX, const GR32 in_dataY, const GR32 in_dataZ)
{
	SetData(in_dataX, in_dataY, in_dataZ);
	return;
}

GVector3Float::GVector3Float(const GR32* const in_data)
{
	SetData(in_data[0], in_data[1], in_data[2]);
	return;
}

GVector3Float::GVector3Float(const GVector3Float& in_src)
{
	(*this) = in_src;
	return;
}

GVector3Float::GVector3Float(const GS32 in_defaultNull)
{
	GASSERT(0 == in_defaultNull, "this is a bit unhealth, but helped with template code");
	SetData(0.0F, 0.0F, 0.0F);
	return;
}

GVector3Float::~GVector3Float()
{
	return;
}

//operators
const GVector3Float& GVector3Float::operator=(const GVector3Float& in_rhs)
{
	m_x = in_rhs.m_x;
	m_y = in_rhs.m_y;
	m_z = in_rhs.m_z;
	return (*this);
}

const GVector3Float& GVector3Float::operator+=(const GVector3Float& in_rhs)
{
	m_x += in_rhs.m_x;
	m_y += in_rhs.m_y;
	m_z += in_rhs.m_z;
	return (*this);
}
const GVector3Float& GVector3Float::operator-=(const GVector3Float& in_rhs)
{
	m_x -= in_rhs.m_x;
	m_y -= in_rhs.m_y;
	m_z -= in_rhs.m_z;
	return (*this);
}
const GVector3Float& GVector3Float::operator*=(const GR32 in_rhs)
{
	m_x *= in_rhs;
	m_y *= in_rhs;
	m_z *= in_rhs;
	return (*this);
}

//public accessors
void GVector3Float::SetData(const GR32 in_dataX, const GR32 in_dataY, const GR32 in_dataZ)
{
	m_x = in_dataX;
	m_y = in_dataY;
	m_z = in_dataZ;
	return;
}

//global operators
const GBOOL operator==(const GVector3Float& in_lhs, const GVector3Float& in_rhs)
{
	if (&in_lhs == &in_rhs)
	{
		return true;
	}
	if (!GMath::AlmostZero(in_lhs.m_x - in_rhs.m_x))
	{
		return false;
	}
	if (!GMath::AlmostZero(in_lhs.m_y - in_rhs.m_y))
	{
		return false;
	}
	if (!GMath::AlmostZero(in_lhs.m_z - in_rhs.m_z))
	{
		return false;
	}
	return true;
}

const GBOOL operator!=(const GVector3Float& in_lhs, const GVector3Float& in_rhs)
{
	return (!(operator==(in_lhs, in_rhs)));
}

const GVector3Float operator+(const GVector3Float& in_lhs, const GVector3Float& in_rhs)
{
	GVector3Float value(in_lhs);
	value += in_rhs;
	return value;
}

const GVector3Float operator-(const GVector3Float& in_lhs, const GVector3Float& in_rhs)
{
	GVector3Float value(in_lhs);
	value -= in_rhs;
	return value;
}

const GVector3Float operator-(const GVector3Float& in_rhs)
{
	return GVector3Float(
		-in_rhs.m_x,
		-in_rhs.m_y,
		-in_rhs.m_z
		);
}

const GVector3Float operator*(const GVector3Float& in_lhs, const GVector3Float& in_rhs)
{
	return GVector3Float(
		in_lhs.m_x * in_rhs.m_x,
		in_lhs.m_y * in_rhs.m_y,
		in_lhs.m_z * in_rhs.m_z
		);
}

const GVector3Float operator*(const GVector3Float& in_lhs, const GR32 in_rhs)
{
	return GVector3Float(
		in_lhs.m_x * in_rhs,
		in_lhs.m_y * in_rhs,
		in_lhs.m_z * in_rhs
		);
}

const GVector3Float operator*(const GR32 in_lhs, const GVector3Float& in_rhs)
{
	return GVector3Float(
		in_lhs * in_rhs.m_x,
		in_lhs * in_rhs.m_y,
		in_lhs * in_rhs.m_z
		);
}

const GVector3Float operator/(const GVector3Float& in_lhs, const GR32 in_rhs)
{
	return (in_lhs * (1.0F / in_rhs));
}

const GVector3Float operator/(const GR32 in_lhs, const GVector3Float& in_rhs)
{
	return ((1.0F / in_lhs) * in_rhs);
}

//global finction overloads
//ratio == 0 is in_low, ratio -> 1.0 is in_high
const GVector3Float Lerp(const GR32 in_ratio, const GVector3Float& in_low, const GVector3Float& in_high)
{
	return GVector3Float(
		GMath::Lerp(in_ratio, in_low.m_x, in_high.m_x),
		GMath::Lerp(in_ratio, in_low.m_y, in_high.m_y),
		GMath::Lerp(in_ratio, in_low.m_z, in_high.m_z)
		);
}

const GR32 DotProduct(const GVector3Float& in_lhs, const GVector3Float& in_rhs)
{
	return (
		(in_lhs.m_x * in_rhs.m_x) + 
		(in_lhs.m_y * in_rhs.m_y) + 
		(in_lhs.m_z * in_rhs.m_z));
}

const GVector3Float CrossProduct(const GVector3Float& in_lhs, const GVector3Float& in_rhs)
{
	const GVector3Float value(
		(in_lhs.m_y * in_rhs.m_z ) - ( in_lhs.m_z * in_rhs.m_y ),
		(in_lhs.m_z * in_rhs.m_x ) - ( in_lhs.m_x * in_rhs.m_z ),
		(in_lhs.m_x * in_rhs.m_y ) - ( in_lhs.m_y * in_rhs.m_x )
		);
	return value;
}

const GVector3Float MakeCrossVector(const GVector3Float& in_src)
{
	const GR32 testDot = DotProduct(GVector3Float::sUnitY, in_src);
	if ((-0.7F < testDot) && (testDot < 0.7F))
	{
		const GVector3Float result = CrossProduct(GVector3Float::sUnitY, in_src);
		return result;
	}

	const GVector3Float resultTwo = CrossProduct(GVector3Float::sUnitZ, in_src);
	return resultTwo;
}

const GVector3Float Normalise(const GVector3Float& in_src)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);

	//bad value case
	if (GMath::AlmostZero(lengthSquared))
	{
		return GVector3Float::sZero;
	}

	//already normalised case
	if (GMath::AlmostZero(lengthSquared - 1.0F))
	{
		return in_src;
	}

	const GR32 mul = 1.0F / GMath::SquareRoot(lengthSquared);
	GVector3Float value = in_src * mul;
	return value;
}

const GVector3Float NormaliseAprox(const GVector3Float& in_src)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);

	//bad value case
	if (GMath::AlmostZero(lengthSquared))
	{
		return GVector3Float::sZero;
	}

	const GR32 mul = GMath::InvertSquareRootAprox(lengthSquared);
	GVector3Float value = in_src * mul;
	return value;
}

const GVector3Float Normalise(const GVector3Float& in_src, GR32& out_length)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);

	//bad value case
	if (GMath::AlmostZero(lengthSquared))
	{
		out_length = 0.0F;
		return GVector3Float::sZero;
	}

	//already normalised case
	if (GMath::AlmostZero(lengthSquared - 1.0F))
	{
		out_length = 1.0F;
		return in_src;
	}

	out_length = GMath::SquareRoot(lengthSquared);
	const GR32 mul = 1.0F / out_length;
	GVector3Float value = in_src * mul;
	return value;
}


const GVector3Float NormaliseAprox(const GVector3Float& in_src, GR32& out_length)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);

	//bad value case
	if (GMath::AlmostZero(lengthSquared))
	{
		out_length = 0.0F;
		return GVector3Float::sZero;
	}

	out_length = GMath::SquareRootAprox(lengthSquared);
	const GR32 mul = 1.0F / out_length;
	GVector3Float value = in_src * mul;
	return value;
}

const GR32 Length(const GVector3Float& in_src)
{
	const GR32 lengthSquared = LengthSquared(in_src);
	const GR32 length = GMath::SquareRoot(lengthSquared);
	return length;
}

const GR32 LengthAprox(const GVector3Float& in_src)
{
	const GR32 lengthSquared = LengthSquared(in_src);
	const GR32 length = GMath::SquareRootAprox(lengthSquared);
	return length;
}
const GR32 LengthSquared(const GVector3Float& in_src)
{
	const GR32 lengthSquared = DotProduct(in_src, in_src);
	return lengthSquared;
}

const GR32 Orthogonalise(GVector3Float& inout_at, GVector3Float& inout_up)
{
	GR32 scale(0.0F);
	inout_at = Normalise(inout_at, scale);
	const GVector3Float right = Normalise(CrossProduct(inout_at, inout_up));
	inout_up = CrossProduct(right, inout_at);
	return scale;
}

const GR32 OrthogonaliseAprox(GVector3Float& inout_at, GVector3Float& inout_up)
{
	GR32 scale(0.0F);
	inout_at = NormaliseAprox(inout_at, scale);
	const GVector3Float right = NormaliseAprox(CrossProduct(inout_at, inout_up));
	inout_up = CrossProduct(right, inout_at);
	return scale;
}

//move to GCollision? GIntersection?
const GBOOL IntersectPlaneRay( 
	GR32& out_ratioOnLine, 
	const GVector3Float& in_planeDirection, //unit normal of plane
	const GVector3Float& in_planePoint, //a poGS32 on the plane
	const GVector3Float& in_lineDirection, //unit normal
	const GVector3Float& in_lineOrigin
	)
{
	const GR32 dom = DotProduct(in_planeDirection, in_lineDirection);
	if (GMath::AlmostZero(dom))
	{
		return false;
	}
	const GR32 num = -DotProduct(in_planeDirection, in_lineOrigin - in_planePoint);
	out_ratioOnLine = num / dom;

	return true;
}

void RayPointClosestPointOfApproach(
	GR32& out_ratioRay,
	const GVector3Float& in_rayOrigin,
	const GVector3Float& in_rayDirection,
	const GVector3Float& in_poGS32
	)
{
	out_ratioRay = DotProduct(in_rayDirection, (in_poGS32 - in_rayOrigin));
	return;
}

/*
http://softsurfer.com/Archive/algorithm_0106/algorithm_0106.htm
// Copyright 2001, softSurfer (www.softsurfer.com)
// This code may be freely used and modified for any purpose
// providing that this copyright notice is included with it.
// SoftSurfer makes no warranty for this code, and cannot be held
// liable for any real or imagined damage resulting from its use.
// Users of this code must verify correctness for their application.

    Vector   u = L1.P1 - L1.P0;
    Vector   v = L2.P1 - L2.P0;
    Vector   w = L1.P0 - L2.P0;
    GR32    a = dot(u,u);        // always >= 0
    GR32    b = dot(u,v);
    GR32    c = dot(v,v);        // always >= 0
    GR32    d = dot(u,w);
    GR32    e = dot(v,w);
    GR32    D = a*c - b*b;       // always >= 0
    GR32    sc, tc;

    // compute the line parameters of the two closest poGS32s
    if (D < SMALL_NUM) {         // the lines are almost parallel
        sc = 0.0;
        tc = (b>c ? d/b : e/c);   // use the largest denominator
    }
    else {
        sc = (b*e - c*d) / D;
        tc = (a*e - b*d) / D;
    }
*/
void RayRayClosestPointOfApproach(
	GR32& out_ratioRayZero,
	GR32& out_ratioRayOne,
	const GVector3Float& in_rayZeroOrigin,
	const GVector3Float& in_rayZeroDirection, //don't assume unit normal
	const GVector3Float& in_rayOneOrigin,
	const GVector3Float& in_rayOneDirection //don't assume unit normal
	)
{
	const GVector3Float originOffset = in_rayZeroOrigin - in_rayOneOrigin;
	const GR32	   a = DotProduct(in_rayZeroDirection, in_rayZeroDirection);        // always >= 0
	const GR32    b = DotProduct(in_rayZeroDirection, in_rayOneDirection);
    const GR32    c = DotProduct(in_rayOneDirection, in_rayOneDirection);        // always >= 0
	const GR32    d = DotProduct(in_rayZeroDirection, originOffset);
	const GR32    e = DotProduct(in_rayOneDirection, originOffset);
	const GR32 denominator = (a*c) - (b*b);

	// compute the line parameters of the two closest poGS32s
	if (GMath::AlmostZero(denominator)) 
	{         // the lines are almost parallel
		out_ratioRayZero = 0.0F;
		out_ratioRayOne = (b>c ? d/b : e/c);
	}
	else 
	{
		out_ratioRayZero = ((b*e) - (c*d)) / denominator;
		out_ratioRayOne = ((a*e) - (b*d)) / denominator;
	}

	return;
}

const GVector3Float ProjectRay(
	const GVector3Float& in_rayOrigin,
	const GVector3Float& in_rayDirection,
	const GR32 in_ratio
	)
{
	const GVector3Float result = in_rayOrigin + (in_rayDirection * in_ratio);
	return result;
}

void SegmentSegmentClosestPointOfApproach(
	GVector3Float& out_segmentA,
	GVector3Float& out_segmentB,
	const GVector3Float& in_segmentAStart,
	const GVector3Float& in_segmentAEnd,
	const GVector3Float& in_segmentBStart,
	const GVector3Float& in_segmentBEnd
	)
{
	const GBOOL segmentACoincident = (in_segmentAEnd == in_segmentAStart);
	const GBOOL segmentBCoincident = (in_segmentBEnd == in_segmentBStart);
	if (segmentACoincident && segmentBCoincident)
	{
		out_segmentA = in_segmentAStart;
		out_segmentB = in_segmentBStart;
	}
	else if (segmentACoincident)
	{
		//ray poGS32
		GR32 segmentBLength = 0.0F;
		const GVector3Float segmentBDirection = Normalise(in_segmentBEnd - in_segmentBStart, segmentBLength);

		GR32 rayRatioB = 0.0F;
		RayPointClosestPointOfApproach(
			rayRatioB,
			in_segmentBStart,
			segmentBDirection,
			in_segmentAStart
			);

		out_segmentA = in_segmentAStart;
		out_segmentB = ProjectRay(in_segmentBStart, segmentBDirection, GMath::Clamp(rayRatioB, 0.0F, segmentBLength));
	}
	else if (segmentBCoincident)
	{
		//ray poGS32
		GR32 segmentALength = 0.0F;
		const GVector3Float segmentADirection = Normalise(in_segmentAEnd - in_segmentAStart, segmentALength);

		GR32 rayRatioA = 0.0F;
		RayPointClosestPointOfApproach(
			rayRatioA,
			in_segmentAStart,
			segmentADirection,
			in_segmentBStart
			);

		out_segmentA = ProjectRay(in_segmentAStart, segmentADirection, GMath::Clamp(rayRatioA, 0.0F, segmentALength));
		out_segmentB = in_segmentBStart;
	}
	else
	{
		//ray ray
		GR32 segmentALength = 0.0F;
		const GVector3Float segmentADirection = Normalise(in_segmentAEnd - in_segmentAStart, segmentALength);
		GR32 segmentBLength = 0.0F;
		const GVector3Float segmentBDirection = Normalise(in_segmentBEnd - in_segmentBStart, segmentBLength);

		GR32 rayRatioA = 0.0F;
		GR32 rayRatioB = 0.0F;
		RayRayClosestPointOfApproach(
			rayRatioA,
			rayRatioB,
			in_segmentAStart,
			segmentADirection,
			in_segmentBStart,
			segmentBDirection
			);

		out_segmentA = ProjectRay(in_segmentAStart, segmentADirection, GMath::Clamp(rayRatioA, 0.0F, segmentALength));
		out_segmentB = ProjectRay(in_segmentBStart, segmentBDirection, GMath::Clamp(rayRatioB, 0.0F, segmentBLength));
	}

	return;
}

const GBOOL MakeAxisAngle(
	GR32& out_radianAngle,
	GVector3Float& out_unitAxis,
	const GVector3Float& in_currentDirection,
	const GVector3Float& in_targetDirection
	)
{
	out_radianAngle = GMath::ACos(DotProduct(Normalise(in_currentDirection), Normalise(in_targetDirection)));
	if (!GMath::NotAlmostZero(out_radianAngle))
	{
		return false;
	}

	out_unitAxis = Normalise(CrossProduct(in_currentDirection, in_targetDirection));

	return true;
}

const GBOOL Valid(const GVector3Float& in_data)
{
	const GR32* const pData = in_data.GetData();
	if ((!GMath::Valid(pData[0])) ||
		(!GMath::Valid(pData[1])) ||
		(!GMath::Valid(pData[2])))
	{
		return false;
	}
	return true;
}