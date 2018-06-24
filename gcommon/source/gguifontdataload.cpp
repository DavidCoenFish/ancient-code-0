//
//  GGuiFontDataLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiFontDataLoad.h"

//flag reflected in tools
struct TGuiFontDataLoadFlag
{
	enum TEnum
	{
		TNone			= 0x00,
		TSupportAnsii	= 0x01, //first byte of the id can be used for ansi characters
		TVisible		= 0x02, //make mesh for this gliph
		TWordWrapBreak	= 0x04  //for wordwrap
	};
};

//constructor
GGuiFontDataLoad::GGuiFontDataLoad(
	const unsigned int in_id,
	const GVector2Float& in_UVTopLeft,
	const GVector2Float& in_UVBottomRight,
	const GVector2Float& in_size,
	const float in_baseLine,
	const int in_flag
	)
	: mID(in_id)
	, mUVTopLeft(in_UVTopLeft)
	, mUVBottomRight(in_UVBottomRight)
	, mSize(in_size)
	, mBaseLine(in_baseLine)
	, mFlag(in_flag)
{
	return;
}

GGuiFontDataLoad::~GGuiFontDataLoad()
{
	return;
}

//public accessors
const bool GGuiFontDataLoad::GetFlagSupportAnsii()const
{
	return (0 != (mFlag & TGuiFontDataLoadFlag::TSupportAnsii));
}

const bool GGuiFontDataLoad::GetFlagVisible()const
{
	return (0 != (mFlag & TGuiFontDataLoadFlag::TVisible));
}

const bool GGuiFontDataLoad::GetFlagWordWrapBreak()const
{
	return (0 != (mFlag & TGuiFontDataLoadFlag::TWordWrapBreak));
}
