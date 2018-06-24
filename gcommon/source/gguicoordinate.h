//
//  GGuiCoordinate.h
//
//  Created by David Coen on 2011 01 17
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiCoordinate_h_
#define _GGuiCoordinate_h_

#include "GVector2Float.h"
#include "GVector2Float.h"

/*
	specify location or dimention of gui item by haing a ratio (percentage of screen) and pixel offset
*/
class GGuiCoordinate
{	
	//constructors
public:
	GGuiCoordinate(
		const GVector2Float& in_originRatio = GVector2Float::sZero,
		const GVector2Float& in_pixelOffset = GVector2Float::sZero
		); 
	GGuiCoordinate(const GGuiCoordinate& in_src);
	~GGuiCoordinate();
	
	//operators
public:
	const GGuiCoordinate& operator=(const GGuiCoordinate& in_rhs);

	//public methods
public:
	//hmmn, same as converting to originRatio(0.0F, 0.0F)?
	const GVector2Float GetAbsoulutePixel(const GVector2Float& in_screenSize)const;

	//public accessors
public:
	const GVector2Float& GetOriginRatio()const{ return mOriginRatio; }
	GVector2Float& GetOriginRatio(){ return mOriginRatio; }

	const GVector2Float& GetPixelOffset()const{ return mPixelOffset; }
	GVector2Float& GetPixelOffset(){ return mPixelOffset; }

	//private members
private:
	GVector2Float mOriginRatio;
	GVector2Float mPixelOffset;

};

/**/
#endif //_GVector3_h_
