//
//  gcolour4byte.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "gcolour4byte.h"

#include "gcolour4float.h"
#include "gmath.h"

/*static*/ const GColour4Byte GColour4Byte::sWhite(255, 255, 255, 255);
/*static*/ const GColour4Byte GColour4Byte::sBlack(0, 0, 0, 255);
/*static*/ const GColour4Byte GColour4Byte::sGrey(127, 127, 127, 255);
/*static*/ const GColour4Byte GColour4Byte::sRed(255, 0, 0, 255);
/*static*/ const GColour4Byte GColour4Byte::sGreen(0, 255, 0, 255);
/*static*/ const GColour4Byte GColour4Byte::sBlue(0, 0, 255, 255);

/*static*/ const GU8 GColour4Byte::FloatToByte(const GR32 in_value)
{
	const GS32 value = GMath::Clamp((GS32)((in_value * 255.0F) + 0.5F), 0, 255);
	return (value & 0xFF);
}

//constructors
GColour4Byte::GColour4Byte(const GU8 in_red, 
						   const GU8 in_green, 
						   const GU8 in_blue, 
						   const GU8 in_alpha
						   )
{
	SetData(in_red, in_green, in_blue, in_alpha);
	return;
}

/*explicit*/ GColour4Byte::GColour4Byte(const GColour4Float& in_src)
{
	m_red = FloatToByte(in_src.m_red);
	m_green = FloatToByte(in_src.m_green);
	m_blue = FloatToByte(in_src.m_blue);
	m_alpha = FloatToByte(in_src.m_alpha);;
	return;
}


GColour4Byte::GColour4Byte(const GColour4Byte& in_src)
{
	(*this) = in_src;
	return;
}

GColour4Byte::~GColour4Byte()
{
	return;
}

//operators
const GColour4Byte& GColour4Byte::operator=(const GColour4Byte& in_rhs)
{
	m_red = in_rhs.m_red;
	m_green = in_rhs.m_green;
	m_blue = in_rhs.m_blue;
	m_alpha = in_rhs.m_alpha;	
	return (*this);	
}
//public accessors
void GColour4Byte::SetData(
	const GU8 in_red, 
	const GU8 in_green, 
	const GU8 in_blue, 
	const GU8 in_alpha
	)
{
	m_red = in_red;
	m_green = in_green;
	m_blue = in_blue;
	m_alpha = in_alpha;
	return;
}
