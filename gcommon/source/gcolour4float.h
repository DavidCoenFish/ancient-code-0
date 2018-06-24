//
//  gcolour4GR32.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _gcolour4GR32_h_
#define _gcolour4GR32_h_

#include "gcommon.h"

class GColour4Byte;

class GColour4Float 
{
	////////////////////////////////////////////////////////////////////////
	//public static methods
public:
	static const GR32 ByteToFloat(const GU8 in_value);

	////////////////////////////////////////////////////////////////////////
	//constructors
public:
	GColour4Float(const GR32 in_red = 0.0F, 
				const GR32 in_green = 0.0F, 
				const GR32 in_blue = 0.0F, 
				const GR32 in_alpha = 1.0F
				);
	GColour4Float(const GColour4Float& in_src);
	explicit GColour4Float(const GColour4Byte& in_src);
	~GColour4Float();
	
	////////////////////////////////////////////////////////////////////////
	//operators
public:
	const GColour4Float& operator=(const GColour4Float& in_rhs);
	const GBOOL operator==(const GColour4Float& in_rhs)const;
	const GBOOL operator!=(const GColour4Float& in_rhs)const;

	////////////////////////////////////////////////////////////////////////
	//public accessors
public:
	const GR32* const GetData()const{ return &m_data[0]; }

	////////////////////////////////////////////////////////////////////////
	//public members
public:
	union
	{
        GR32 m_data[4];
        struct
        {
			//openGL memory
            GR32 m_red;
            GR32 m_green;
            GR32 m_blue;
            GR32 m_alpha;
        };
	};
	////////////////////////////////////////////////////////////////////////
	//static public members
public:
	static const GColour4Float sWhite;
	static const GColour4Float sBlack;
	static const GColour4Float sGrey;
	static const GColour4Float sRed;
	static const GColour4Float sGreen;
	static const GColour4Float sBlue;
	
};

#endif //_gcolour4GR32_h_
