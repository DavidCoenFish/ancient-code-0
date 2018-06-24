//
//  gmatrix9float.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GMatrix9Float_h_
#define _GMatrix9Float_h_

#include "gvector3float.h"

class GQuaternion4Float;
class GVector3Float;

class GMatrix9Float
{
	//constructors
public:
	GMatrix9Float(const GR32 in_data_0_0 = 0.0F, const GR32 in_data_0_1 = 0.0F, const GR32 in_data_0_2 = 0.0F,
			 const GR32 in_data_1_0 = 0.0F, const GR32 in_data_1_1 = 0.0F, const GR32 in_data_1_2 = 0.0F,
			 const GR32 in_data_2_0 = 0.0F, const GR32 in_data_2_1 = 0.0F, const GR32 in_data_2_2 = 0.0F
			 );
	GMatrix9Float(const GR32* const in_data);
	GMatrix9Float(const GMatrix9Float& in_src);
	~GMatrix9Float();
	
	//operators
public:
	const GMatrix9Float& operator=(const GMatrix9Float& in_rhs);
	
	//public methods
public:
	GMatrix9Float& TransposeSelf();
	const GMatrix9Float ReturnInverse()const;

	//public accessors
public:
	void SetData(const GR32 in_data_0_0, const GR32 in_data_0_1, const GR32 in_data_0_2,
		const GR32 in_data_1_0, const GR32 in_data_1_1, const GR32 in_data_1_2,
		const GR32 in_data_2_0, const GR32 in_data_2_1, const GR32 in_data_2_2
		);
	const GR32* const GetData()const{ return m_data; }
	const GVector3Float GetAt()const;
	const GVector3Float GetUp()const;

	//public members
public:
	union
	{
        GR32 m_data[9];
        struct
        {
			//Make matrix data order match opengl documentation
			GR32 m_0_0;
            GR32 m_1_0;
            GR32 m_2_0;
            GR32 m_0_1;
            GR32 m_1_1;
            GR32 m_2_1;
            GR32 m_0_2;
            GR32 m_1_2;
            GR32 m_2_2;
        };
	};
	//static public members
public:
	static const GMatrix9Float sIdentity;
	
};
/**/

//global operators
const GMatrix9Float operator*(const GMatrix9Float& in_lhs, const GMatrix9Float& in_rhs); 
const GVector3Float operator*(const GVector3Float& in_lhs, const GMatrix9Float& in_rhs);
const GMatrix9Float& operator*=(GMatrix9Float& in_lhs, const GMatrix9Float& in_rhs);
	
const GMatrix9Float GMatrix9FloatConstructAtUp( 
	const GVector3Float& in_targetAt, 
	const GVector3Float& in_targetUp,
	const GVector3Float& in_baseAt, 
	const GVector3Float& in_baseUp
	);
const GMatrix9Float GMatrix9FloatConstructAxisAngle(const GVector3Float& in_axis, const GR32 in_angleRad);


#endif //_GMatrix9Float_h_