//
//  GComponentLerp.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 01
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "GComponentLerp.h"
#include "GMath.h"

//constructor
GComponentLerp::GComponentLerp(
	const float in_duration
	)
: mTime(0.0F)
, mDuration(in_duration)
, mRatio(0.0F)
, mType(TType::TLinear)
, mFinished(false)
{
	return;
}

GComponentLerp::~GComponentLerp()
{
	return;
}

//public methods
void GComponentLerp::Tick(const float in_timeDelta)
{
	if (!mFinished)
	{
		mTime += in_timeDelta;
		if (mDuration <= mTime)
		{
			mFinished = true;
			mRatio = 1.0F;
			mDuration = 0.0F;
			mTime = 0.0F;
		}
		else if (mDuration)
		{
			mRatio = mTime / mDuration;
			if (TType::TEaseInOut == mType)
			{
				mRatio = GMath::RatioEaseInOut(mRatio);
			}
		}
	}
	return;
}

//public accessors
void GComponentLerp::SetLerp(const float in_duration, const TType::TEnum in_type)
{
	mRatio = 0.0F;
	mType = in_type;
	mTime = 0.0F;
	mDuration = in_duration;
	mFinished = false;
	Tick(0.0F);
	return;
}
