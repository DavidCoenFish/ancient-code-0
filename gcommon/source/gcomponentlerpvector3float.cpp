//
//  GComponentLerpVector3Float.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 01
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "GComponentLerpVector3Float.h"
#include "GMath.h"

//constructor
GComponentLerpVector3Float::GComponentLerpVector3Float(
	const GVector3Float& in_value,
	const float in_duration,
	const GVector3Float& in_targetValue
	)
: mValue(in_value)
, mValueLerpOld(in_value)
, mValueLerpTarget(in_targetValue)
, mLerp()
{
	mLerp.reset(new GComponentLerp(in_duration));
	return;
}

GComponentLerpVector3Float::~GComponentLerpVector3Float()
{
	return;
}

//public methods
void GComponentLerpVector3Float::Tick(const float in_timeDelta)
{
	if (mLerp->GetActive())
	{
		mLerp->Tick(in_timeDelta);
		const float ratio = mLerp->GetRatio();
		mValue = Lerp(ratio, mValueLerpOld, mValueLerpTarget);
	}

	return;
}

//public accessors
void GComponentLerpVector3Float::SetValue(
	const GVector3Float& in_targetValue, 
	const float in_duration, 
	const GComponentLerp::TType::TEnum in_lerpType
	)
{
	if (in_duration)
	{
		mValueLerpOld = mValue;
		mValueLerpTarget = in_targetValue;
		mLerp->SetLerp(in_duration, in_lerpType);
		Tick(0.0F);
	}
	else
	{
		mValue = in_targetValue;
		mLerp->SetLerp(0.0F);
	}

	return;
}

const bool GComponentLerpVector3Float::GetActive()const
{
	if (mLerp)
	{
		return mLerp->GetActive();
	}
	return false;
}
