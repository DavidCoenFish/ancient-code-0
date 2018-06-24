//
//  GGuiCoordinate.cpp
//
//  Created by David Coen on 2011 01 17
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiCoordinate.h"

#include "GAssert.h"

//constructors
GGuiCoordinate::GGuiCoordinate(
	const GVector2Float& in_originRatio,
	const GVector2Float& in_pixelOffset							   
	)
: mOriginRatio(in_originRatio)
, mPixelOffset(in_pixelOffset)
{
	return;
}

GGuiCoordinate::GGuiCoordinate(const GGuiCoordinate& in_src)
: mOriginRatio()
, mPixelOffset()
{
	(*this) = in_src;
	return;
}

GGuiCoordinate::~GGuiCoordinate()
{
	return;
}

//operators
const GGuiCoordinate& GGuiCoordinate::operator=(const GGuiCoordinate& in_rhs)
{
	if (this != &in_rhs)
	{
		mOriginRatio = in_rhs.mOriginRatio;
		mPixelOffset = in_rhs.mPixelOffset;
	}
	return (*this);
}

//public methods
const GVector2Float GGuiCoordinate::GetAbsoulutePixel(const GVector2Float& in_screenSize)const
{
	const GVector2Float result(
		mPixelOffset.m_x + ((mOriginRatio.m_x * in_screenSize.m_x)),
		mPixelOffset.m_y + ((mOriginRatio.m_y * in_screenSize.m_y))
		);
	return result;
}
