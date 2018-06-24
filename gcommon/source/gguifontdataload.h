//
//  GGuiFontDataLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiFontDataLoad_h_
#define _GGuiFontDataLoad_h_

#include <boost/noncopyable.hpp>
#include "GVector2Float.h"

/**/
class GGuiFontDataLoad : public boost::noncopyable
{
	//constructor
public:
	GGuiFontDataLoad(
		const unsigned int in_id,
		const GVector2Float& in_UVTopLeft,
		const GVector2Float& in_UVBottomRight,
		const GVector2Float& in_size,
		const float in_baseLine,
		const int in_flag
		);
	~GGuiFontDataLoad();

	//public accessors
public:
	const unsigned int GetID()const{ return mID; }
	const GVector2Float& GetUVTopLeft()const{ return mUVTopLeft; }
	const GVector2Float& GetUVBottomRight()const{ return mUVBottomRight; }
	const GVector2Float& GetSize()const{ return mSize; }
	const float GetBaseLine()const{ return mBaseLine; } //distance from top to common line (waterline) of font
	
	const bool GetFlagSupportAnsii()const;
	const bool GetFlagVisible()const;
	const bool GetFlagWordWrapBreak()const;

	//private members;
private:
	const unsigned int mID;
	const GVector2Float mUVTopLeft;
	const GVector2Float mUVBottomRight;
	const GVector2Float mSize;
	const float mBaseLine; //distance from top to common line (waterline) of font
	const int mFlag;


};

#endif 