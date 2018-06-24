//
//  gcolour4GR32.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gcolour4float.h"
#include "gcolour4byte.h"

/*static*/ const GColour4Float GColour4Float::sWhite(1.0F, 1.0F, 1.0F, 1.0F);
/*static*/ const GColour4Float GColour4Float::sBlack(0.0F, 0.0F, 0.0F, 1.0F);
/*static*/ const GColour4Float GColour4Float::sGrey(0.5F, 0.5F, 0.5F, 1.0F);
/*static*/ const GColour4Float GColour4Float::sRed(1.0F, 0.0F, 0.0F, 1.0F);
/*static*/ const GColour4Float GColour4Float::sGreen(0.0F, 1.0F, 0.0F, 1.0F);
/*static*/ const GColour4Float GColour4Float::sBlue(0.0F, 0.0F, 1.0F, 1.0F);

////////////////////////////////////////////////////////////////////////
//public static methods
/*static*/ const GR32 GColour4Float::ByteToFloat(const GU8 in_value)
{
	return (((GR32)in_value) / 255.0F);
}

//constructors
GColour4Float::GColour4Float(const GR32 in_red, 
			 const GR32 in_green, 
			 const GR32 in_blue, 
			 const GR32 in_alpha
			 )
{
	m_red = in_red;
	m_green = in_green;
	m_blue = in_blue;
	m_alpha = in_alpha;
	return;
}

GColour4Float::GColour4Float(const GColour4Float& in_src)
{
	(*this) = in_src;
	return;
}

/*explicit*/ GColour4Float::GColour4Float(const GColour4Byte& in_src)
{
	m_red = ByteToFloat(in_src.m_red);
	m_green = ByteToFloat(in_src.m_green);
	m_blue = ByteToFloat(in_src.m_blue);;
	m_alpha = ByteToFloat(in_src.m_alpha);;
	return;
}

GColour4Float::~GColour4Float()
{
	return;
}

//operators
const GColour4Float& GColour4Float::operator=(const GColour4Float& in_rhs)
{
	m_red = in_rhs.m_red;
	m_green = in_rhs.m_green;
	m_blue = in_rhs.m_blue;
	m_alpha = in_rhs.m_alpha;	
	return (*this);	
}

const GBOOL GColour4Float::operator==(const GColour4Float& in_rhs)const
{
	for (int index = 0; index < 4; ++index)
	{
		if (m_data[index] != in_rhs.m_data[index])
		{
			return false;
		}
	}
	return true;
}

const GBOOL GColour4Float::operator!=(const GColour4Float& in_rhs)const
{
	return (!((*this) == (in_rhs)));
}
