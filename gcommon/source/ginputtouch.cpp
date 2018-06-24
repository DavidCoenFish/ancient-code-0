//
//  ginputtouch.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "ginputtouch.h"
#include "gassert.h"

//constructor
GInputTouch::GInputTouch(
	const unsigned int in_touchId,
	const GVector2Float& in_touchLocation,
	const GVector2Float& in_touchDelta,
	const bool in_touchBegin,
	const bool in_touchEnd,
	const bool in_touchCancel
	)
	: mTouchId(in_touchId)
	, mTouchLocation(in_touchLocation)
	, mTouchDelta(in_touchDelta)
	, mTouchBegin(in_touchBegin)
	, mTouchCancel(in_touchCancel)
	, mTouchEnd(in_touchEnd)
{	
	return;
}

GInputTouch::~GInputTouch()
{
	return;
}

//public methods
void GInputTouch::UpdateLocation(const GVector2Float& in_newLocation, const GBOOL in_begin)
{
	if (in_begin)
	{
		mTouchDelta.SetData(0.0F, 0.0F);
		mTouchLocation = in_newLocation;
	}
	else
	{
		mTouchDelta += (in_newLocation - mTouchLocation);
		mTouchLocation = in_newLocation;
	}
	mTouchBegin = in_begin;
	return;
}

void GInputTouch::ClearDelta()
{
	mTouchDelta = GVector2Float::sZero;
	return;
}

static void LocalOrientate(
	GVector2Float& out_result,
	const GVector2Float& in_location,
	const GApplicationWindowType::TOrientation::TEnum in_orientation
	)
{
	switch (in_orientation)
	{
	default:
		out_result = in_location;
		break;
	case GApplicationWindowType::TOrientation::TThreeOclock:
		out_result.SetData(1.0F - in_location.m_y, in_location.m_x);
		break;
	case GApplicationWindowType::TOrientation::TSixOclock:
		out_result.SetData(1.0F - in_location.m_x, 1.0F - in_location.m_y);
		break;
	case GApplicationWindowType::TOrientation::TNineOclock:
		out_result.SetData(in_location.m_y, 1.0F - in_location.m_x);
		break;
	}
	return;
}

const GVector2Float GInputTouch::GetTouchLocationOrientation(const GApplicationWindowType::TOrientation::TEnum in_orientation)const
{
	switch (in_orientation)
	{
	default:
		break;
	case GApplicationWindowType::TOrientation::TThreeOclock:
		return GVector2Float(1.0F - mTouchLocation.m_y, mTouchLocation.m_x);
	case GApplicationWindowType::TOrientation::TSixOclock:
		return GVector2Float(1.0F - mTouchLocation.m_x, 1.0F - mTouchLocation.m_y);
	case GApplicationWindowType::TOrientation::TNineOclock:
		return GVector2Float(mTouchLocation.m_y, 1.0F - mTouchLocation.m_x);
	}
	return mTouchLocation;
}

const GVector2Float GInputTouch::GetTouchDeltaOrientation(const GApplicationWindowType::TOrientation::TEnum in_orientation)const
{
	switch (in_orientation)
	{
	default:
		break;
	case GApplicationWindowType::TOrientation::TThreeOclock:
		return GVector2Float(-(mTouchDelta.m_y), mTouchDelta.m_x);
	case GApplicationWindowType::TOrientation::TSixOclock:
		return -mTouchDelta;
	case GApplicationWindowType::TOrientation::TNineOclock:
		return GVector2Float(mTouchDelta.m_y, -(mTouchDelta.m_x));
	}
	return mTouchDelta;
}
