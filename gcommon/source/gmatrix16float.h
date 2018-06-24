//
//  gmatrix16float.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gmatrix16float_h_
#define _gmatrix16float_h_

#include "gvector3float.h"

class GQuaternion4Float;
class GVector3Float;
class GMatrix9Float;

class GMatrix16Float
{
	//constructors
public:
	GMatrix16Float(const GR32 in_data_0_0 = 0.0F, const GR32 in_data_0_1 = 0.0F, const GR32 in_data_0_2 = 0.0F, const GR32 in_data_0_3 = 0.0F,
			 const GR32 in_data_1_0 = 0.0F, const GR32 in_data_1_1 = 0.0F, const GR32 in_data_1_2 = 0.0F, const GR32 in_data_1_3 = 0.0F,
			 const GR32 in_data_2_0 = 0.0F, const GR32 in_data_2_1 = 0.0F, const GR32 in_data_2_2 = 0.0F, const GR32 in_data_2_3 = 0.0F,
			 const GR32 in_data_3_0 = 0.0F, const GR32 in_data_3_1 = 0.0F, const GR32 in_data_3_2 = 0.0F, const GR32 in_data_3_3 = 0.0F
			 );
	GMatrix16Float(const GR32* const in_data);
	GMatrix16Float(const GMatrix16Float& in_src);
	~GMatrix16Float();
	
	//operators
public:
	const GMatrix16Float& operator=(const GMatrix16Float& in_rhs);
	const GMatrix16Float& operator*=(const GR32 in_rhs);
	
	//public methods
public:
	void Compose(
		const GVector3Float& in_position,
		const GQuaternion4Float& in_rotation,
		const GVector3Float& in_scale
		);

	void Decompose(
		GVector3Float& out_position,
		GQuaternion4Float& out_rotation,
		GVector3Float& out_scale
		);

	GMatrix16Float& TransposeSelf();
	GMatrix16Float& InverseSelf();
	const GMatrix16Float ReturnInverse()const;

	GMatrix9Float GetRotation()const;
	void SetRotation(const GMatrix9Float& in_rotation);

	//public accessors
public:
	void SetData(const GR32 in_data_0_0, const GR32 in_data_0_1, const GR32 in_data_0_2, const GR32 in_data_0_3,
		const GR32 in_data_1_0, const GR32 in_data_1_1, const GR32 in_data_1_2, const GR32 in_data_1_3,
		const GR32 in_data_2_0, const GR32 in_data_2_1, const GR32 in_data_2_2, const GR32 in_data_2_3,
		const GR32 in_data_3_0, const GR32 in_data_3_1, const GR32 in_data_3_2, const GR32 in_data_3_3
		);
	const GR32* const GetData()const{ return m_data; }

	const GVector3Float GetAt()const;
	const GVector3Float GetUp()const;
	const GVector3Float GetPosition()const;
	void SetPosition(const GVector3Float& in_position);

	//public members
public:
	union
	{
        GR32 m_data[16];
        struct
        {
			//Make matrix data order match opengl documentation
			GR32 m_0_0;
            GR32 m_1_0;
            GR32 m_2_0;
            GR32 m_3_0;
            GR32 m_0_1;
            GR32 m_1_1;
            GR32 m_2_1;
            GR32 m_3_1;
            GR32 m_0_2;
            GR32 m_1_2;
            GR32 m_2_2;
            GR32 m_3_2;
            GR32 m_0_3;
            GR32 m_1_3;
            GR32 m_2_3;
            GR32 m_3_3;
        };
	};

	//static public members
public:
	static const GMatrix16Float sIdentity;
	
};
/**/

//global operators
const GMatrix16Float operator*(const GMatrix16Float& in_lhs, const GMatrix16Float& in_rhs); 
//const GVector3Float operator*(const GMatrix16Float& in_lhs, const GVector3Float& in_rhs); //upcast vec3 to (x, y, z, 1.0) hahha, no
const GVector3Float operator*(const GVector3Float& in_lhs, const GMatrix16Float& in_rhs); //upcast vec3 to (x, y, z, 1.0)

const GVector3Float GMatrix16FloatMultiplyNoTranslate(const GVector3Float& in_lhs, const GMatrix16Float& in_rhs);

const GMatrix16Float GMatrix16FloatConstructAtUp( 
	const GVector3Float& in_targetAt, 
	const GVector3Float& in_targetUp,
	const GVector3Float& in_baseAt = GVector3Float::sUnitY, 
	const GVector3Float& in_baseUp = GVector3Float::sUnitZ,
	const GVector3Float& in_position = GVector3Float::sZero
	);
void GMatrix16FloatDecomposeAtUp(
	GVector3Float& out_at, 
	GVector3Float& out_up,
	GVector3Float& out_position,
	const GMatrix16Float& in_matrix
	);

const GMatrix16Float GMatrix16FloatConstructAxisAngle(const GVector3Float& in_axis, const GR32 in_angleRad);

const GBOOL Valid(const GMatrix16Float& in_data);

//cheap invert, no skew or scale support
const GMatrix16Float GMatrix16FloatInvertOrthogonal(const GMatrix16Float& in_src);

#endif //_gmatrix16float_h_