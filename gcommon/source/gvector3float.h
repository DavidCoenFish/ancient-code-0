//
//  gvector3float.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GVector3Float_h_
#define _GVector3Float_h_

#include "gcommon.h"

/**/
class GVector3Float
{	
	//constructors
public:
	GVector3Float(const GR32 in_dataX = 0.0F, const GR32 in_dataY = 0.0F, const GR32 in_dataZ = 0.0F);
	GVector3Float(const GR32* const in_data);
	GVector3Float(const GVector3Float& in_src);
	GVector3Float(const GS32 in_defaultNull); //asserts if not 0
	~GVector3Float();
	
	//operators
public:
	const GVector3Float& operator=(const GVector3Float& in_rhs);

	const GVector3Float& operator+=(const GVector3Float& in_rhs);
	const GVector3Float& operator-=(const GVector3Float& in_rhs); 
	const GVector3Float& operator*=(const GR32 in_rhs); 

	//public accessors
public:
	void SetData(const GR32 in_dataX, const GR32 in_dataY, const GR32 in_dataZ);
	const GR32* const GetData()const{ return m_data; }

	//public members
public:
	union
	{
        float m_data[3];
        struct
        {
            float m_x;
            float m_y;
            float m_z;
        };
	};

	//static public members
public:
	static const GVector3Float sZero;
	static const GVector3Float sUnit;
	static const GVector3Float sUnitX;
	static const GVector3Float sUnitY;
	static const GVector3Float sUnitZ;
	
};

/**/
//global operators
const GBOOL operator==(const GVector3Float& in_lhs, const GVector3Float& in_rhs); 
const GBOOL operator!=(const GVector3Float& in_lhs, const GVector3Float& in_rhs); 

const GVector3Float operator+(const GVector3Float& in_lhs, const GVector3Float& in_rhs); 
//const GVector3Float& operator+=(GVector3Float& in_lhs, const GVector3Float& in_rhs); 

const GVector3Float operator-(const GVector3Float& in_lhs, const GVector3Float& in_rhs); 
//const GVector3Float& operator-=(GVector3Float& in_lhs, const GVector3Float& in_rhs); 

const GVector3Float operator-(const GVector3Float& in_rhs); 

const GVector3Float operator*(const GVector3Float& in_lhs, const GVector3Float& in_rhs); 
const GVector3Float operator*(const GVector3Float& in_lhs, const GR32 in_rhs); 
const GVector3Float operator*(const GR32 in_lhs, const GVector3Float& in_rhs); 

const GVector3Float operator/(const GVector3Float& in_lhs, const GR32 in_rhs); 
const GVector3Float operator/(const GR32 in_lhs, const GVector3Float& in_rhs); 

//global function overloads
const GVector3Float Lerp(const GR32 in_ratio, const GVector3Float& in_lhs, const GVector3Float& in_rhs); 

const GR32 DotProduct(const GVector3Float& in_lhs, const GVector3Float& in_rhs); 
const GVector3Float CrossProduct(const GVector3Float& in_lhs, const GVector3Float& in_rhs); 
const GVector3Float MakeCrossVector(const GVector3Float& in_src); 

const GVector3Float Normalise(const GVector3Float& in_src);
const GVector3Float NormaliseAprox(const GVector3Float& in_src);
const GVector3Float Normalise(const GVector3Float& in_src, GR32& out_length);
const GVector3Float NormaliseAprox(const GVector3Float& in_src, GR32& out_length);

const GR32 Length(const GVector3Float& in_src);
const GR32 LengthAprox(const GVector3Float& in_src);
const GR32 LengthSquared(const GVector3Float& in_src);

const GR32 Orthogonalise(GVector3Float& inout_at, GVector3Float& inout_up);
const GR32 OrthogonaliseAprox(GVector3Float& inout_at, GVector3Float& inout_up);

const GBOOL IntersectPlaneRay(
	GR32& out_ratioOnLine, 
	const GVector3Float& in_planeDirection, //unit normal of plane
	const GVector3Float& in_planePoint, //a poGS32 on the plane
	const GVector3Float& in_rayDirection, //unit normal
	const GVector3Float& in_rayOrigin
	);

void RayPointClosestPointOfApproach(
	GR32& out_ratioRay,
	const GVector3Float& in_rayOrigin,
	const GVector3Float& in_rayDirection,
	const GVector3Float& in_poGS32
	);

void RayRayClosestPointOfApproach(
	GR32& out_ratioRayZero,
	GR32& out_ratioRayOne,
	const GVector3Float& in_rayZeroOrigin,
	const GVector3Float& in_rayZeroDirection,
	const GVector3Float& in_rayOneOrigin,
	const GVector3Float& in_rayOneDirection
	);

const GVector3Float ProjectRay(
	const GVector3Float& in_rayOrigin,
	const GVector3Float& in_rayDirection,
	const GR32 in_ratio
	);

void SegmentSegmentClosestPointOfApproach(
	GVector3Float& out_segmentA,
	GVector3Float& out_segmentB,
	const GVector3Float& in_segmentAStart,
	const GVector3Float& in_segmentAEnd,
	const GVector3Float& in_segmentBStart,
	const GVector3Float& in_segmentBEnd
	);

//return false if equivalent to zero angle
const GBOOL MakeAxisAngle(
	GR32& out_radianAngle,
	GVector3Float& out_unitAxis,
	const GVector3Float& in_currentDirection,
	const GVector3Float& in_targetDirection
	);

const GBOOL Valid(const GVector3Float& in_data);

#endif //_GVector3Float_h_
