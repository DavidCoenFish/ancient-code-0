//
//  GQuaternion4Float.h
//
//  Created by David Coen on 30/08/10.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GQuaternion4Float_h_
#define _GQuaternion4Float_h_

/**/
class GQuaternion4Float
{
	//typedef
private:
	struct TIndex
	{
		enum TEnum 
		{
			TDataW = 0,
			TDataX,
			TDataY,
			TDataZ,
			TCount
		};
	};
	
	//constructors
public:
	GQuaternion4Float(const float in_dataW = 1.0F, const float in_dataX = 0.0F, const float in_dataY = 0.0F, const float in_dataZ = 0.0F);
	GQuaternion4Float(const GQuaternion4Float& in_src);
	~GQuaternion4Float();
	
	//operators
public:
	const GQuaternion4Float& operator=(const GQuaternion4Float& in_rhs);
	
	//public accessors
public:
	const float* const GetData()const{ return m_data; }

	//public members
public:
	union
	{
        float m_data[4];
        struct
        {
            float m_w;
            float m_x;
            float m_y;
            float m_z;
        };
	};

	//static public members
public:
	static const GQuaternion4Float sIdentity;
	
};
/**/
#endif //_GQuaternion_h_
