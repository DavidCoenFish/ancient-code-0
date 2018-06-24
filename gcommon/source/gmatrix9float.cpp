//
//  gmatrix9float.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmatrix9float.h"

#include "gvector3float.h"
#include "gmathmatrix.h"
#include "gmath.h"

#include <boost/swap.hpp>

#define DSC_INLINE_MATRIX_MUL

/*static*/ const GMatrix9Float GMatrix9Float::sIdentity(1.0F, 0.0F, 0.0F,
														  0.0F, 1.0F, 0.0F,
														  0.0F, 0.0F, 1.0F);


//constructors
GMatrix9Float::GMatrix9Float(const GR32 in_data_0_0, const GR32 in_data_0_1, const GR32 in_data_0_2,
	const GR32 in_data_1_0, const GR32 in_data_1_1, const GR32 in_data_1_2,
	const GR32 in_data_2_0, const GR32 in_data_2_1, const GR32 in_data_2_2
	)
{
	SetData(in_data_0_0, in_data_0_1, in_data_0_2,
		in_data_1_0, in_data_1_1, in_data_1_2,
		in_data_2_0, in_data_2_1, in_data_2_2
		);
	return;
}
GMatrix9Float::GMatrix9Float(const GR32* const in_data)
{
	for (GS32 index = 0; index < 9; ++index)
	{
		m_data[index] = in_data[index];
	}
	return;

}

GMatrix9Float::GMatrix9Float(const GMatrix9Float& in_src)
{
	(*this) = in_src;
	return;
}

GMatrix9Float::~GMatrix9Float()
{
	return;
}
	
//operators
const GMatrix9Float& GMatrix9Float::operator=(const GMatrix9Float& in_rhs)
{
	for (GS32 index = 0; index < 9; ++index)
	{
		m_data[index] = in_rhs.m_data[index];
	}
	return (*this);
}

//public methods
GMatrix9Float& GMatrix9Float::TransposeSelf()
{
	std::swap(m_0_1, m_1_0);
	std::swap(m_0_2, m_2_0);
	std::swap(m_1_2, m_2_1);

	return (*this);
}

const GMatrix9Float GMatrix9Float::ReturnInverse()const
{
	GMatrix9Float result;
	GMathMatrix<GR32>::Inverse4(&result.m_data[0], &m_data[0]);

	return result;
}

//public accessors
void GMatrix9Float::SetData(const GR32 in_data_0_0, const GR32 in_data_0_1, const GR32 in_data_0_2,
	const GR32 in_data_1_0, const GR32 in_data_1_1, const GR32 in_data_1_2,
	const GR32 in_data_2_0, const GR32 in_data_2_1, const GR32 in_data_2_2
	)
{
	m_0_0 = in_data_0_0;
	m_1_0 = in_data_1_0;
	m_2_0 = in_data_2_0;

	m_0_1 = in_data_0_1;
	m_1_1 = in_data_1_1;
	m_2_1 = in_data_2_1;

	m_0_2 = in_data_0_2;
	m_1_2 = in_data_1_2;
	m_2_2 = in_data_2_2;

	return;
}

const GVector3Float GMatrix9Float::GetAt()const
{
	const GVector3Float result(
		m_0_2,
		m_1_2,
		m_2_2
		);
	return result;
}

const GVector3Float GMatrix9Float::GetUp()const
{
	const GVector3Float result(
		m_0_1,
		m_1_1,
		m_2_1
		);
	return result;
}

//global operators
const GMatrix9Float operator*(const GMatrix9Float& in_lhs, const GMatrix9Float& in_rhs)
{
	GR32 value[9];
#ifdef DSC_INLINE_MATRIX_MUL
	const GR32* const lhsData = in_lhs.GetData();
	const GR32* const rhsData = in_rhs.GetData();
	value[ 0] = (lhsData[ 0] * rhsData[ 0]) + (lhsData[ 1] * rhsData[ 3]) + (lhsData[ 2] * rhsData[ 6]);
	value[ 1] = (lhsData[ 0] * rhsData[ 1]) + (lhsData[ 1] * rhsData[ 4]) + (lhsData[ 2] * rhsData[ 7]);
	value[ 2] = (lhsData[ 0] * rhsData[ 2]) + (lhsData[ 1] * rhsData[ 5]) + (lhsData[ 2] * rhsData[ 8]);

	value[ 3] = (lhsData[ 3] * rhsData[ 0]) + (lhsData[ 4] * rhsData[ 3]) + (lhsData[ 5] * rhsData[ 6]);
	value[ 4] = (lhsData[ 3] * rhsData[ 1]) + (lhsData[ 4] * rhsData[ 4]) + (lhsData[ 5] * rhsData[ 7]);
	value[ 5] = (lhsData[ 3] * rhsData[ 2]) + (lhsData[ 4] * rhsData[ 5]) + (lhsData[ 5] * rhsData[ 8]);

	value[ 6] = (lhsData[ 6] * rhsData[ 0]) + (lhsData[ 7] * rhsData[ 3]) + (lhsData[ 8] * rhsData[ 6]);
	value[ 7] = (lhsData[ 6] * rhsData[ 1]) + (lhsData[ 7] * rhsData[ 4]) + (lhsData[ 8] * rhsData[ 7]);
	value[ 8] = (lhsData[ 6] * rhsData[ 2]) + (lhsData[ 7] * rhsData[ 5]) + (lhsData[ 8] * rhsData[ 8]);
#else
	GMathMatrix<GR32>::MatrixMul( 
		in_lhs.GetData(),
		in_rhs.GetData(), 
		3, 
		3, 
		3, 
		&value[0]
		);
#endif
	return GMatrix9Float(&value[0]);
}

const GVector3Float operator*(const GVector3Float& in_lhs, const GMatrix9Float& in_rhs)
{
	GR32 value[3];
#ifdef DSC_INLINE_MATRIX_MUL
	const GR32* const lhsData = in_lhs.GetData();
	const GR32* const rhsData = in_rhs.GetData();
	value[ 0] = (lhsData[ 0] * rhsData[ 0]) + (lhsData[ 1] * rhsData[ 3]) + (lhsData[ 2] * rhsData[ 6]);
	value[ 1] = (lhsData[ 0] * rhsData[ 1]) + (lhsData[ 1] * rhsData[ 4]) + (lhsData[ 2] * rhsData[ 7]);
	value[ 2] = (lhsData[ 0] * rhsData[ 2]) + (lhsData[ 1] * rhsData[ 5]) + (lhsData[ 2] * rhsData[ 8]);
#else
	GMathMatrix<GR32>::MatrixMul( 
		in_lhs.GetData(),
		in_rhs.GetData(), 
		3, 
		1, 
		3, 
		&value[0]
		);
#endif
	return GVector3Float(value[0], value[1], value[2]);
}

const GMatrix9Float& operator*=(GMatrix9Float& in_lhs, const GMatrix9Float& in_rhs)
{
	in_lhs = (in_lhs * in_rhs);
	return in_lhs;
}	

/*
  from _Mathematics for computer graphics p.171
 mapping a pair of vectors onto another pair, u,x being one pair, a,y being another of unit vectors at same angle
 u.x = a.y = cos(\), with sin(\) != 0
 a roation matrix sending u,x to a,y given by
               a 
 M = [u v w ][ b ]
               c
 d = | x * u | = | y * a | = | sin(\) |
 if 90deg = u to x, cos(\) = 0, sin(\) = 1
 v = ( x * u ) / d
 b = ( y * a ) / d
 w = ( x - u( cos(\) ) ) / d
 c = ( y - a( cos(\) ) ) / d
*/
const GMatrix9Float GMatrix9FloatConstructAtUp( 
	const GVector3Float& in_targetAt, 
	const GVector3Float& in_targetUp,
	const GVector3Float& in_baseAt, 
	const GVector3Float& in_baseUp
	)
{
	GMatrix9Float returnMatrix;

	const GVector3Float crossBaseUpAt = CrossProduct(in_baseUp, in_baseAt);
	const GVector3Float crossTargetUpAt = CrossProduct(in_targetUp, in_targetAt);

	return GMatrix9Float(
		(in_baseAt.m_x * in_targetAt.m_x) + (crossBaseUpAt.m_x * crossTargetUpAt.m_x) + (in_baseUp.m_x * in_targetUp.m_x),
		(in_baseAt.m_y * in_targetAt.m_x) + (crossBaseUpAt.m_y * crossTargetUpAt.m_x) + (in_baseUp.m_y * in_targetUp.m_x),
		(in_baseAt.m_z * in_targetAt.m_x) + (crossBaseUpAt.m_z * crossTargetUpAt.m_x) + (in_baseUp.m_z * in_targetUp.m_x),

		(in_baseAt.m_x * in_targetAt.m_y) + (crossBaseUpAt.m_x * crossTargetUpAt.m_y) + (in_baseUp.m_x * in_targetUp.m_y),
		(in_baseAt.m_y * in_targetAt.m_y) + (crossBaseUpAt.m_y * crossTargetUpAt.m_y) + (in_baseUp.m_y * in_targetUp.m_y),
		(in_baseAt.m_z * in_targetAt.m_y) + (crossBaseUpAt.m_z * crossTargetUpAt.m_y) + (in_baseUp.m_z * in_targetUp.m_y),

		(in_baseAt.m_x * in_targetAt.m_z) + (crossBaseUpAt.m_x * crossTargetUpAt.m_z) + (in_baseUp.m_x * in_targetUp.m_z),
		(in_baseAt.m_y * in_targetAt.m_z) + (crossBaseUpAt.m_y * crossTargetUpAt.m_z) + (in_baseUp.m_y * in_targetUp.m_z),
		(in_baseAt.m_z * in_targetAt.m_z) + (crossBaseUpAt.m_z * crossTargetUpAt.m_z) + (in_baseUp.m_z * in_targetUp.m_z)
		);
}

/*
  http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm
*/
const GMatrix9Float GMatrix9FloatConstructAxisAngle(const GVector3Float& in_axis, const GR32 in_angleRad)
{
	const GVector3Float localAxis = Normalise(in_axis);
	const GR32 axis_x = localAxis.m_x;
	const GR32 axis_y = localAxis.m_y;
	const GR32 axis_z = localAxis.m_z;

	const GR32 c = GMath::Cos( in_angleRad );
	const GR32 s = GMath::Sin( in_angleRad );
	const GR32 t = 1.0F - c;

	const GR32 tmp1_01 = axis_x * axis_y * t;
	const GR32 tmp2_01 = axis_z * s;

	const GR32 tmp1_02 = axis_x * axis_z * t;
	const GR32 tmp2_02 = axis_y * s;
  
	const GR32 tmp1_21 = axis_y * axis_z * t;
	const GR32 tmp2_21 = axis_x * s;

	return GMatrix9Float(
		c + axis_x * axis_x * t, tmp1_01 - tmp2_01, tmp1_02 + tmp2_02,
		tmp1_01 + tmp2_01, c + axis_y * axis_y * t, tmp1_21 - tmp2_21,
		tmp1_02 - tmp2_02, tmp1_21 + tmp2_21, c + axis_z * axis_z * t
		);
}
