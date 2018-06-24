//
//  GComponentLerpFloat.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 01
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "GComponentLerpFloat.h"
#include "GMath.h"

//constructor
GComponentLerpFloat::GComponentLerpFloat(
	const float in_value,
	const float in_duration,
	const float in_targetValue
	)
: mValue(in_value)
, mValueLerpOld(in_value)
, mValueLerpTarget(in_targetValue)
, mLerp()
{
	mLerp.reset(new GComponentLerp(in_duration));
	return;
}

GComponentLerpFloat::~GComponentLerpFloat()
{
	return;
}

//public methods
void GComponentLerpFloat::Tick(const float in_timeDelta)
{
	if (mLerp->GetActive())
	{
		mLerp->Tick(in_timeDelta);
		const float ratio = mLerp->GetRatio();
		mValue = GMath::Lerp(ratio, mValueLerpOld, mValueLerpTarget);
	}

	return;
}

//public accessors
void GComponentLerpFloat::SetValue(
	const float in_targetValue, 
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

//wrap old value so we take shortest path around a circle
void GComponentLerpFloat::SetValueRadian(
	const float in_targetValue, 
	const float in_duration, 
	const GComponentLerp::TType::TEnum in_lerpType
	)
{
	if (in_duration)
	{
		mValueLerpTarget = in_targetValue;

		const float shortPath = GMath::ClampRadian(mValueLerpTarget - mValue);
		mValueLerpOld = mValueLerpTarget - shortPath;

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

const bool GComponentLerpFloat::GetActive()const
{
	if (mLerp)
	{
		return mLerp->GetActive();
	}
	return false;
}
