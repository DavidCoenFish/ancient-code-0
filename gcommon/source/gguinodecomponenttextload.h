//
//  GGuiNodeComponentTextLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeComponentTextLoad_h_
#define _GGuiNodeComponentTextLoad_h_

#include <boost/noncopyable.hpp>
#include "GColour4Byte.h"

class GGuiNodeComponentTextLoad : public boost::noncopyable
{
	//constructor
public:
	GGuiNodeComponentTextLoad(
		const float in_scale,
		const float in_horizontalCharPad,
		const GColour4Byte& in_colour,
		const int in_flag,
		const int in_maximumCharCount,
		const char* const in_name,
		const char* const in_text,
		const char* const in_fontName
		);
	~GGuiNodeComponentTextLoad();

	//public accessors
public:
	const float GetScale()const{ return mScale; }
	const float GetHorizontalCharPad()const{ return mHorizontalCharPad; }
	const GColour4Byte& GetColour()const{ return mColour; }
	const bool GetFlagLineWrap()const;
	const bool GetFlagVisible()const;
	const int GetMaximumCharCount()const{ return mMaximumCharCount; }
	const char* const GetName()const{ return mName; }
	const char* const GetText()const{ return mText; }
	const char* const GetFontName()const{ return mFontName; }

	//private members;
private:
	const float mScale;
	const float mHorizontalCharPad;
	const GColour4Byte mColour;
	const int mFlag;
	const int mMaximumCharCount;
	const char* const mName;
	const char* const mText;
	const char* const mFontName;

};

#endif 