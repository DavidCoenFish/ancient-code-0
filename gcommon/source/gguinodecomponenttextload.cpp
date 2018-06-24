//
//  GGuiNodeComponentTextLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentTextLoad.h"

struct TGuiNodeComponentTextLoadFlag
{
	enum TEnum
	{
		TNone		= 0x0000,
		TLineWrap	= 0x0001,
		TVisible	= 0x0002,
	};
};

//constructor
GGuiNodeComponentTextLoad::GGuiNodeComponentTextLoad(
	const float in_scale,
	const float in_horizontalCharPad,
	const GColour4Byte& in_colour,
	const int in_flag,
	const int in_maximumCharCount,
	const char* const in_name,
	const char* const in_text,
	const char* const in_fontName
	)
	: mScale(in_scale)
	, mHorizontalCharPad(in_horizontalCharPad)
	, mColour(in_colour)
	, mFlag(in_flag)
	, mMaximumCharCount(in_maximumCharCount)
	, mName(in_name)
	, mText(in_text)
	, mFontName(in_fontName)
{
	return;
}

GGuiNodeComponentTextLoad::~GGuiNodeComponentTextLoad()
{
	return;
}

//public accessors
const bool GGuiNodeComponentTextLoad::GetFlagLineWrap()const
{
	return (0 != (mFlag & TGuiNodeComponentTextLoadFlag::TLineWrap));
}

const bool GGuiNodeComponentTextLoad::GetFlagVisible()const
{
	return (0 != (mFlag & TGuiNodeComponentTextLoadFlag::TVisible));
}
