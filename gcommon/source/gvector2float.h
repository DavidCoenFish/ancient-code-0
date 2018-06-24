//
//  gvector2float.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gvector2float_h_
#define _gvector2float_h_

#include "gcommon.h"

/**/
class GVector2Float
{
	//constructors
public:
	GVector2Float(const GR32 in_dataX = 0.0F, const GR32 in_dataY = 0.0F);
	GVector2Float(const GVector2Float& in_src);
	~GVector2Float();
	
	//operators
public:
	const GVector2Float& operator=(const GVector2Float& in_rhs);
	const GVector2Float& operator+=(const GVector2Float& in_rhs);
	const GVector2Float& operator-=(const GVector2Float& in_rhs); 
	const GVector2Float& operator*=(const GR32 in_rhs); 
	const GVector2Float& operator/=(const GR32 in_rhs); 
	
	//public accessors
public:
	void SetData(const GR32 in_dataX, const GR32 in_dataY);
	const GR32* const GetData()const{ return m_data; }

	//public members
public:
	union
	{
        float m_data[2];
        struct
        {
            float m_x;
            float m_y;
        };
	};

	//static public members
public:
	static const GVector2Float sZero;
	static const GVector2Float sUnit;
	static const GVector2Float sUnitX;
	static const GVector2Float sUnitY;
	
};

/**/
//global operators
const GBOOL operator==(const GVector2Float& in_lhs, const GVector2Float& in_rhs); 
const GBOOL operator!=(const GVector2Float& in_lhs, const GVector2Float& in_rhs); 

const GVector2Float operator+(const GVector2Float& in_lhs, const GVector2Float& in_rhs); 

const GVector2Float operator-(const GVector2Float& in_lhs, const GVector2Float& in_rhs); 
const GVector2Float operator-(const GVector2Float& in_rhs); 

const GVector2Float operator*(const GVector2Float& in_lhs, const GVector2Float& in_rhs);
const GVector2Float operator*(const GVector2Float& in_lhs, const GR32 in_rhs); 
const GVector2Float operator*(const GR32 in_lhs, const GVector2Float& in_rhs); 

const GVector2Float operator/(const GVector2Float& in_lhs, const GR32 in_rhs); 

const GR32 DotProduct(const GVector2Float& in_lhs, const GVector2Float& in_rhs);
const GR32 Length(const GVector2Float& in_src);
const GR32 LengthSquared(const GVector2Float& in_src);
const GR32 LengthAprox(const GVector2Float& in_src);

const GVector2Float Normalise(const GVector2Float& in_src);
const GVector2Float NormaliseAprox(const GVector2Float& in_src);
const GVector2Float Normalise(const GVector2Float& in_src, GR32& out_length);
const GVector2Float NormaliseAprox(const GVector2Float& in_src, GR32& out_length);

const GBOOL BoundPointOverlap(
    const GVector2Float& in_lhsBoundLow,
    const GVector2Float& in_lhsBoundHigh,
    const GVector2Float& in_point
	);

const GBOOL BoundBoundOverlap(
    const GVector2Float& in_lhsBoundLow,
    const GVector2Float& in_lhsBoundHigh,
    const GVector2Float& in_rhsBoundLow,
    const GVector2Float& in_rhsBoundHigh
	);

#endif //_gvector2float_h_
