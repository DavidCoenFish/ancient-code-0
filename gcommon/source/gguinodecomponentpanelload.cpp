//
//  GGuiNodeComponentPanelLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentPanelLoad.h"

struct TGuiNodeComponentPanelLoadFlag
{
	enum TEnum
	{
		TNone		= 0x00,
		TColour1	= 0x01,
		TColour4	= 0x02,
		TUV			= 0x04,
		TVisible	= 0x08
	};
};

//constructor
GGuiNodeComponentPanelLoad::GGuiNodeComponentPanelLoad(
	const char* const in_name,
	const GColour4Byte* const in_arrayColour,
	const GVector2Float* const in_arrayUVPair,
	const int in_flag
	)
	: mName(in_name)
	, mArrayColour(in_arrayColour)
	, mArrayUVPair(in_arrayUVPair)
	, mFlag(in_flag)
{
	return;
}

GGuiNodeComponentPanelLoad::~GGuiNodeComponentPanelLoad()
{
	return;
}

const bool GGuiNodeComponentPanelLoad::GetFlagColour1()const
{
	return (0 != (mFlag & TGuiNodeComponentPanelLoadFlag::TColour1));
}

const bool GGuiNodeComponentPanelLoad::GetFlagColour4()const
{
	return (0 != (mFlag & TGuiNodeComponentPanelLoadFlag::TColour4));
}

const bool GGuiNodeComponentPanelLoad::GetFlagUv()const
{
	return (0 != (mFlag & TGuiNodeComponentPanelLoadFlag::TUV));
}

const bool GGuiNodeComponentPanelLoad::GetFlagVisible()const
{
	return (0 != (mFlag & TGuiNodeComponentPanelLoadFlag::TVisible));
}

