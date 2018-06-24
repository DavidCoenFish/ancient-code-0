//
//  gcolour4byte.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GColour4Byte_h_
#define _GColour4Byte_h_

#include "gcommon.h"

class GColour4Float;

class GColour4Byte 
{
	////////////////////////////////////////////////////////////////////////
	//public static methods
public:
	static const GU8 FloatToByte(const GR32 in_value);
	
	////////////////////////////////////////////////////////////////////////
	//constructors
public:
	GColour4Byte(const GU8 in_red = 0, 
				const GU8 in_green = 0, 
				const GU8 in_blue = 0, 
				const GU8 in_alpha = 255
				);
	GColour4Byte(const GColour4Byte& in_src);
	explicit GColour4Byte(const GColour4Float& in_src);
	~GColour4Byte();
	
	////////////////////////////////////////////////////////////////////////
	//operators
public:
	const GColour4Byte& operator=(const GColour4Byte& in_rhs);
	
	////////////////////////////////////////////////////////////////////////
	//public accessors
public:
	void SetData(
		const GU8 in_red, 
		const GU8 in_green, 
		const GU8 in_blue, 
		const GU8 in_alpha
		);

	////////////////////////////////////////////////////////////////////////
	//public members
public:
	union
	{
        GU8 m_data[4];
        struct
        {
            GU8 m_blue;
            GU8 m_green;
            GU8 m_red;
            GU8 m_alpha;
        };
	};

	////////////////////////////////////////////////////////////////////////
	//static public members
public:
	static const GColour4Byte sWhite;
	static const GColour4Byte sBlack;
	static const GColour4Byte sGrey;
	static const GColour4Byte sRed;
	static const GColour4Byte sGreen;
	static const GColour4Byte sBlue;
	
};

#endif //_GColour4Byte_h_
