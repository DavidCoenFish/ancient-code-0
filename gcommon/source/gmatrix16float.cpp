//
//  gmatrix16float.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gmatrix16float.h"

#include "gvector3float.h"
#include "gmathmatrix.h"
#include "gmath.h"
#include "gmatrix9float.h"

#include <boost/swap.hpp>

#define DSC_INLINE_MATRIX_MUL

/*
matching openGL documentation
		0_0, 0_1, 0_2, 0_3(x),
		1_0, 1_1, 1_2, 1_3(y),
		2_0, 2_1, 2_2, 2_3(z),
		3_0, 3_1, 3_2, 3_3
*/

/*static*/ const GMatrix16Float GMatrix16Float::sIdentity(1.0F, 0.0F, 0.0F, 0.0F,
														  0.0F, 1.0F, 0.0F, 0.0F,
														  0.0F, 0.0F, 1.0F, 0.0F,
														  0.0F, 0.0F, 0.0F, 1.0F);


//constructors
GMatrix16Float::GMatrix16Float(const GR32 in_data_0_0, const GR32 in_data_0_1, const GR32 in_data_0_2, const GR32 in_data_0_3,
	const GR32 in_data_1_0, const GR32 in_data_1_1, const GR32 in_data_1_2, const GR32 in_data_1_3,
	const GR32 in_data_2_0, const GR32 in_data_2_1, const GR32 in_data_2_2, const GR32 in_data_2_3,
	const GR32 in_data_3_0, const GR32 in_data_3_1, const GR32 in_data_3_2, const GR32 in_data_3_3
	)
{
	SetData(in_data_0_0, in_data_0_1, in_data_0_2, in_data_0_3,
		in_data_1_0, in_data_1_1, in_data_1_2, in_data_1_3,
		in_data_2_0, in_data_2_1, in_data_2_2, in_data_2_3,
		in_data_3_0, in_data_3_1, in_data_3_2, in_data_3_3
		);
	return;
}
GMatrix16Float::GMatrix16Float(const GR32* const in_data)
{
	for (GS32 index = 0; index < 16; ++index)
	{
		m_data[index] = in_data[index];
	}
	return;

}

GMatrix16Float::GMatrix16Float(const GMatrix16Float& in_src)
{
	(*this) = in_src;
	return;
}

GMatrix16Float::~GMatrix16Float()
{
	return;
}
	
//operators
const GMatrix16Float& GMatrix16Float::operator=(const GMatrix16Float& in_rhs)
{
	for (GS32 index = 0; index < 16; ++index)
	{
		m_data[index] = in_rhs.m_data[index];
	}
	return (*this);
}

const GMatrix16Float& GMatrix16Float::operator*=(const GR32 in_rhs)
{
	for (GS32 index = 0; index < 16; ++index)
	{
		m_data[index] *= in_rhs;
	}

	return (*this);
}

//public methods
void GMatrix16Float::Decompose(GVector3Float& out_position,
   GQuaternion4Float& out_rotation,
   GVector3Float& out_scale
   )
{
	return;
}

GMatrix16Float&  GMatrix16Float::TransposeSelf()
{
	std::swap(m_0_1, m_1_0);
	std::swap(m_0_2, m_2_0);
	std::swap(m_0_3, m_3_0);
	std::swap(m_1_2, m_2_1);
	std::swap(m_1_3, m_3_1);
	std::swap(m_2_3, m_3_2);

	return (*this);
}
GMatrix16Float& GMatrix16Float::InverseSelf()
{
	(*this) = ReturnInverse();
	return (*this);
}


const GMatrix16Float GMatrix16Float::ReturnInverse()const
{
	GMatrix16Float result;
	GMathMatrix<GR32>::Inverse4(&result.m_data[0], &m_data[0]);

	return result;
}

GMatrix9Float GMatrix16Float::GetRotation()const
{
	return GMatrix9Float(
		m_0_0, m_0_1, m_0_2,
		m_1_0, m_1_1, m_1_2,
		m_2_0, m_2_1, m_2_2
	);
}

void GMatrix16Float::SetRotation(const GMatrix9Float& in_rotation)
{
	m_0_0 = in_rotation.m_0_0;
	m_1_0 = in_rotation.m_1_0;
	m_2_0 = in_rotation.m_2_0;
	m_0_1 = in_rotation.m_0_1;
	m_1_1 = in_rotation.m_1_1;
	m_2_1 = in_rotation.m_2_1;
	m_0_2 = in_rotation.m_0_2;
	m_1_2 = in_rotation.m_1_2;
	m_2_2 = in_rotation.m_2_2;
	return;
}


//public accessors
void GMatrix16Float::SetData(const GR32 in_data_0_0, const GR32 in_data_0_1, const GR32 in_data_0_2, const GR32 in_data_0_3,
	const GR32 in_data_1_0, const GR32 in_data_1_1, const GR32 in_data_1_2, const GR32 in_data_1_3,
	const GR32 in_data_2_0, const GR32 in_data_2_1, const GR32 in_data_2_2, const GR32 in_data_2_3,
	const GR32 in_data_3_0, const GR32 in_data_3_1, const GR32 in_data_3_2, const GR32 in_data_3_3
	)
{
	m_0_0 = in_data_0_0;
	m_1_0 = in_data_1_0;
	m_2_0 = in_data_2_0;
	m_3_0 = in_data_3_0;

	m_0_1 = in_data_0_1;
	m_1_1 = in_data_1_1;
	m_2_1 = in_data_2_1;
	m_3_1 = in_data_3_1;

	m_0_2 = in_data_0_2;
	m_1_2 = in_data_1_2;
	m_2_2 = in_data_2_2;
	m_3_2 = in_data_3_2;

	m_0_3 = in_data_0_3;
	m_1_3 = in_data_1_3;
	m_2_3 = in_data_2_3;
	m_3_3 = in_data_3_3;

	return;
}

const GVector3Float GMatrix16Float::GetAt()const
{
	const GVector3Float result(
		m_0_2,
		m_1_2,
		m_2_2
		);
	return result;
}

const GVector3Float GMatrix16Float::GetUp()const
{
	const GVector3Float result(
		m_0_1,
		m_1_1,
		m_2_1
		);
	return result;
}

const GVector3Float GMatrix16Float::GetPosition()const
{
	const GVector3Float result(
		m_0_3,
		m_1_3,
		m_2_3
		);
	return result;
}

void GMatrix16Float::SetPosition(const GVector3Float& in_position)
{
	m_0_3 = in_position.m_x;
	m_1_3 = in_position.m_y;
	m_2_3 = in_position.m_z;
	return;
}

//global operators
const GMatrix16Float operator*(const GMatrix16Float& in_lhs, const GMatrix16Float& in_rhs)
{
	GR32 value[16];
#ifdef DSC_INLINE_MATRIX_MUL
	const GR32* const lhsData = in_lhs.GetData();
	const GR32* const rhsData = in_rhs.GetData();
	value[ 0] = (lhsData[ 0] * rhsData[ 0]) + (lhsData[ 1] * rhsData[ 4]) + (lhsData[ 2] * rhsData[ 8]) + (lhsData[ 3] * rhsData[12]);
	value[ 1] = (lhsData[ 0] * rhsData[ 1]) + (lhsData[ 1] * rhsData[ 5]) + (lhsData[ 2] * rhsData[ 9]) + (lhsData[ 3] * rhsData[13]);
	value[ 2] = (lhsData[ 0] * rhsData[ 2]) + (lhsData[ 1] * rhsData[ 6]) + (lhsData[ 2] * rhsData[10]) + (lhsData[ 3] * rhsData[14]);
	value[ 3] = (lhsData[ 0] * rhsData[ 3]) + (lhsData[ 1] * rhsData[ 7]) + (lhsData[ 2] * rhsData[11]) + (lhsData[ 3] * rhsData[15]);

	value[ 4] = (lhsData[ 4] * rhsData[ 0]) + (lhsData[ 5] * rhsData[ 4]) + (lhsData[ 6] * rhsData[ 8]) + (lhsData[ 7] * rhsData[12]);
	value[ 5] = (lhsData[ 4] * rhsData[ 1]) + (lhsData[ 5] * rhsData[ 5]) + (lhsData[ 6] * rhsData[ 9]) + (lhsData[ 7] * rhsData[13]);
	value[ 6] = (lhsData[ 4] * rhsData[ 2]) + (lhsData[ 5] * rhsData[ 6]) + (lhsData[ 6] * rhsData[10]) + (lhsData[ 7] * rhsData[14]);
	value[ 7] = (lhsData[ 4] * rhsData[ 3]) + (lhsData[ 5] * rhsData[ 7]) + (lhsData[ 6] * rhsData[11]) + (lhsData[ 7] * rhsData[15]);

	value[ 8] = (lhsData[ 8] * rhsData[ 0]) + (lhsData[ 9] * rhsData[ 4]) + (lhsData[10] * rhsData[ 8]) + (lhsData[11] * rhsData[12]);
	value[ 9] = (lhsData[ 8] * rhsData[ 1]) + (lhsData[ 9] * rhsData[ 5]) + (lhsData[10] * rhsData[ 9]) + (lhsData[11] * rhsData[13]);
	value[10] = (lhsData[ 8] * rhsData[ 2]) + (lhsData[ 9] * rhsData[ 6]) + (lhsData[10] * rhsData[10]) + (lhsData[11] * rhsData[14]);
	value[11] = (lhsData[ 8] * rhsData[ 3]) + (lhsData[ 9] * rhsData[ 7]) + (lhsData[10] * rhsData[11]) + (lhsData[11] * rhsData[15]);

	value[12] = (lhsData[12] * rhsData[ 0]) + (lhsData[13] * rhsData[ 4]) + (lhsData[14] * rhsData[ 8]) + (lhsData[15] * rhsData[12]);
	value[13] = (lhsData[12] * rhsData[ 1]) + (lhsData[13] * rhsData[ 5]) + (lhsData[14] * rhsData[ 9]) + (lhsData[15] * rhsData[13]);
	value[14] = (lhsData[12] * rhsData[ 2]) + (lhsData[13] * rhsData[ 6]) + (lhsData[14] * rhsData[10]) + (lhsData[15] * rhsData[14]);
	value[15] = (lhsData[12] * rhsData[ 3]) + (lhsData[13] * rhsData[ 7]) + (lhsData[14] * rhsData[11]) + (lhsData[15] * rhsData[15]);
#else
	GMathMatrix<GR32>::MatrixMul( 
		in_lhs.GetData(),
		in_rhs.GetData(), 
		4, 
		4, 
		4, 
		&value[0]
		);
#endif
	return GMatrix16Float(&value[0]);
}

const GVector3Float GMatrix16FloatMultiplyNoTranslate(const GVector3Float& in_lhs, const GMatrix16Float& in_rhs)
{
#ifdef DSC_INLINE_MATRIX_MUL
	GR32 value[3];
	const GR32* const lhsData = in_lhs.GetData();
	const GR32* const rhsData = in_rhs.GetData();
	value[ 0] = (lhsData[ 0] * rhsData[ 0]) + (lhsData[ 1] * rhsData[ 4]) + (lhsData[ 2] * rhsData[ 8]);
	value[ 1] = (lhsData[ 0] * rhsData[ 1]) + (lhsData[ 1] * rhsData[ 5]) + (lhsData[ 2] * rhsData[ 9]);
	value[ 2] = (lhsData[ 0] * rhsData[ 2]) + (lhsData[ 1] * rhsData[ 6]) + (lhsData[ 2] * rhsData[10]);
#else
	GR32 source[4];
	GR32 value[4];
	//promote
	source[0] = in_lhs.m_x;
	source[1] = in_lhs.m_y;
	source[2] = in_lhs.m_z;
	source[3] = 0.0F;

	//< m, n > * < p, m > = < p, n >
	GMathMatrix<GR32>::MatrixMul( 
		&source[0], 
		in_rhs.GetData(),
		4, //m
		1, //n
		4, //p
		&value[0]
		);
#endif

	return GVector3Float(&value[0]);
}

//{
//	const GMatrix9Float subMatrix = in_lhs.GetRotation();
//	const GVector3Float result = subMatrix * in_rhs;
//	return result;
//}

const GVector3Float operator*(const GVector3Float& in_lhs, const GMatrix16Float& in_rhs)
{
#ifdef DSC_INLINE_MATRIX_MUL
	const GR32* const lhsData = in_lhs.GetData();
	const GR32* const rhsData = in_rhs.GetData();
	return GVector3Float(
		(lhsData[ 0] * rhsData[ 0]) + (lhsData[ 1] * rhsData[ 4]) + (lhsData[ 2] * rhsData[ 8]) + rhsData[12],
		(lhsData[ 0] * rhsData[ 1]) + (lhsData[ 1] * rhsData[ 5]) + (lhsData[ 2] * rhsData[ 9]) + rhsData[13],
		(lhsData[ 0] * rhsData[ 2]) + (lhsData[ 1] * rhsData[ 6]) + (lhsData[ 2] * rhsData[10]) + rhsData[14]
		);
#else
	GR32 source[4];
	GR32 value[4];
	//promote
	source[0] = in_lhs.m_x;
	source[1] = in_lhs.m_y;
	source[2] = in_lhs.m_z;
	source[3] = 1.0F;

	//< m, n > * < p, m > = < p, n >
	GMathMatrix<GR32>::MatrixMul( 
		&source[0], 
		in_rhs.GetData(),
		4, //m
		1, //n
		4, //p
		&value[0]
		);
	return GVector3Float(&value[0]);
#endif
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
const GMatrix16Float GMatrix16FloatConstructAtUp( 
	const GVector3Float& in_targetAt, 
	const GVector3Float& in_targetUp,
	const GVector3Float& in_baseAt, 
	const GVector3Float& in_baseUp,
	const GVector3Float& in_position
	)
{
	////is this a stablity issue
	//const GR32 dotResultA = DotProduct(in_baseAt, in_targetAt);
	//const GR32 dotResultU = DotProduct(in_baseUp, in_targetUp);
	//if (((dotResultA < -0.999F) || (0.999F < dotResultA)) &&
	//	((dotResultU < -0.999F) || (0.999F < dotResultU)))
	//{
	//	GMatrix16Float result(GMatrix16Float::sIdentity);
	//	result.SetPosition(in_position);
	//	return result;
	//}

	const GVector3Float crossBaseUpAt = CrossProduct(in_baseUp, in_baseAt);
	const GVector3Float crossTargetUpAt = CrossProduct(in_targetUp, in_targetAt);

	return GMatrix16Float(
		(in_baseAt.m_x * in_targetAt.m_x) + (crossBaseUpAt.m_x * crossTargetUpAt.m_x) + (in_baseUp.m_x * in_targetUp.m_x),
		(in_baseAt.m_y * in_targetAt.m_x) + (crossBaseUpAt.m_y * crossTargetUpAt.m_x) + (in_baseUp.m_y * in_targetUp.m_x),
		(in_baseAt.m_z * in_targetAt.m_x) + (crossBaseUpAt.m_z * crossTargetUpAt.m_x) + (in_baseUp.m_z * in_targetUp.m_x),
		in_position.m_x,

		(in_baseAt.m_x * in_targetAt.m_y) + (crossBaseUpAt.m_x * crossTargetUpAt.m_y) + (in_baseUp.m_x * in_targetUp.m_y),
		(in_baseAt.m_y * in_targetAt.m_y) + (crossBaseUpAt.m_y * crossTargetUpAt.m_y) + (in_baseUp.m_y * in_targetUp.m_y),
		(in_baseAt.m_z * in_targetAt.m_y) + (crossBaseUpAt.m_z * crossTargetUpAt.m_y) + (in_baseUp.m_z * in_targetUp.m_y),
		in_position.m_y,

		(in_baseAt.m_x * in_targetAt.m_z) + (crossBaseUpAt.m_x * crossTargetUpAt.m_z) + (in_baseUp.m_x * in_targetUp.m_z),
		(in_baseAt.m_y * in_targetAt.m_z) + (crossBaseUpAt.m_y * crossTargetUpAt.m_z) + (in_baseUp.m_y * in_targetUp.m_z),
		(in_baseAt.m_z * in_targetAt.m_z) + (crossBaseUpAt.m_z * crossTargetUpAt.m_z) + (in_baseUp.m_z * in_targetUp.m_z),
		in_position.m_z,

		0.0F,
		0.0F,
		0.0F,
		1.0F
		);
}

void GMatrix16FloatDecomposeAtUp(
	GVector3Float& out_at, 
	GVector3Float& out_up,
	GVector3Float& out_position,
	const GMatrix16Float& in_matrix
	)
{
	out_at = in_matrix.GetAt();
	out_up = in_matrix.GetUp();
	out_position = in_matrix.GetPosition();
	return;
}



/*
  http://www.euclideanspace.com/maths/geometry/rotations/conversions/angleToMatrix/index.htm
	matrix[ HMatrixR4::IndexGet( 0, 0 ) ] = c + axis_x * axis_x * t;
	matrix[ HMatrixR4::IndexGet( 1, 1 ) ] = c + axis_y * axis_y * t;
	matrix[ HMatrixR4::IndexGet( 2, 2 ) ] = c + axis_z * axis_z * t;

	HREAL tmp1 = axis_x * axis_y * t;
	HREAL tmp2 = axis_z * s;
	matrix[ HMatrixR4::IndexGet( 1, 0 ) ] = tmp1 + tmp2;
	matrix[ HMatrixR4::IndexGet( 0, 1 ) ] = tmp1 - tmp2;
	tmp1 = axis_x * axis_z * t;
	tmp2 = axis_y * s;
	matrix[ HMatrixR4::IndexGet( 2, 0 ) ] = tmp1 - tmp2;
	matrix[ HMatrixR4::IndexGet( 0, 2 ) ] = tmp1 + tmp2;    
	tmp1 = axis_y * axis_z * t;
	tmp2 = axis_x * s;
	matrix[ HMatrixR4::IndexGet( 2, 1 ) ] = tmp1 + tmp2;
	matrix[ HMatrixR4::IndexGet( 1, 2 ) ] = tmp1 - tmp2;
*/
const GMatrix16Float GMatrix16FloatConstructAxisAngle(const GVector3Float& in_axis, const GR32 in_angleRad)
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

	return GMatrix16Float(
		c + axis_x * axis_x * t,		tmp1_01 - tmp2_01,			tmp1_02 + tmp2_02,			0.0F,
		tmp1_01 + tmp2_01,				c + axis_y * axis_y * t,	tmp1_21 - tmp2_21,			0.0F,
		tmp1_02 - tmp2_02,				tmp1_21 + tmp2_21,			c + axis_z * axis_z * t,	0.0F,
		0.0F,							0.0F,						0.0F,						1.0F
		);
}

const GBOOL Valid(const GMatrix16Float& in_data)
{
	const GR32* const pData = in_data.GetData();
	if ((!GMath::Valid(pData[0])) ||
		(!GMath::Valid(pData[1])) ||
		(!GMath::Valid(pData[2])) ||
		(!GMath::Valid(pData[3])) ||
		(!GMath::Valid(pData[4])) ||
		(!GMath::Valid(pData[5])) ||
		(!GMath::Valid(pData[6])) ||
		(!GMath::Valid(pData[7])) ||
		(!GMath::Valid(pData[8])) ||
		(!GMath::Valid(pData[9])) ||
		(!GMath::Valid(pData[10])) ||
		(!GMath::Valid(pData[11])) ||
		(!GMath::Valid(pData[12])) ||
		(!GMath::Valid(pData[13])) ||
		(!GMath::Valid(pData[14])) ||
		(!GMath::Valid(pData[15])))
	{
		return false;
	}
	return true;
}

//cheap invert, no skew or scale support
const GMatrix16Float GMatrix16FloatInvertOrthogonal(const GMatrix16Float& in_src)
{
	const GVector3Float position = in_src.GetPosition();
	GMatrix16Float transpose(in_src);
	transpose.SetPosition(GVector3Float::sZero);
	transpose.TransposeSelf();
	transpose.SetPosition(-position);

	return transpose;
}
