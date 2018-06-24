//
//  GAnimationWrapper.cpp
//
//  Created by David Coen on 2011 01 06
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationWrapper.h"
#include "GAnimationManagerLoad.h"
#include "GAnimationStreamInfoLoad.h"
#include "GAnimationStreamDataLoad.h"
#include "GAnimationLoad.h"
#include "GAnimationType.h"
#include "GAnimationKeyLoad.h"
#include "GAssert.h"
#include "GMath.h"
#include "GVector3Float.h"
#include "GAnimationInstance.h"

struct TAnimationWrapperFlag
{
	enum TEnum
	{
		TNone			= 0x00,
		TManualTick		= 0x01,
		TManualFade		= 0x02,
		TFadeIn			= 0x04,
		TFadeOut		= 0x08,
		TFinished		= 0x10
	};
};

//constructor
GAnimationWrapper::GAnimationWrapper(
	TPointerAnimationInstance& inout_animationInstance,
	const float in_weight,
	const float in_tickRate,
	const float in_fadeRate,
	const float in_fadeInDuration,
	const bool in_manualTick,
	const bool in_manualFade
	)
	: mAnimationInstance(inout_animationInstance)
	, mFadeDuration(in_fadeInDuration)
	, mFadeTime(0.0F)
	, mWeight(in_weight)
	, mTickRate(in_tickRate)
	, mFadeRate(in_fadeRate)
	, mFlag(0)
{
	if (0.0F != mFadeDuration)
	{
		mFlag |= TAnimationWrapperFlag::TFadeIn;
	}
	if (in_manualTick)
	{
		mFlag |= TAnimationWrapperFlag::TManualTick;
	}
	if (in_manualFade)
	{
		mFlag |= TAnimationWrapperFlag::TManualFade;
	}

	//pump through a tick to be sure of initial state (ie, mWeight)
	TickFade(0.0F);

	return;
}

GAnimationWrapper::~GAnimationWrapper()
{
	return;
}

//public methods
const bool GAnimationWrapper::GetFinished()const
{
	return (0 != (mFlag & TAnimationWrapperFlag::TFinished));
}

const bool GAnimationWrapper::SimpleTick(const float in_timeDelta, const GAnimationManagerLoad& in_animationManagerLoad)
{
	TickFade(in_timeDelta);
	TickAnimation(in_timeDelta, in_animationManagerLoad);

	return !GetFinished();
}

void GAnimationWrapper::ApplyToStreamPosition(
	GVector3Float& inout_position,
	const GAnimationManagerLoad& in_animationManagerLoad,
	const int in_streamIndex
	)const
{
	//GASSERT(false == GetFinished(), "invalid code path");
	if (!mAnimationInstance->HasStream(in_streamIndex))
	{
		return;
	}

	if (1.0F == mWeight)
	{
		mAnimationInstance->ApplyToStreamPosition(
			inout_position,
			in_animationManagerLoad,
			in_streamIndex
			);
	}
	else
	{
		GVector3Float value;
		mAnimationInstance->ApplyToStreamPosition(
			value,
			in_animationManagerLoad,
			in_streamIndex
			);

		inout_position = Lerp(mWeight, inout_position, value);
	}

	return;
}

void GAnimationWrapper::ApplyToStreamRotation(
	GVector3Float& inout_at,
	GVector3Float& inout_up,
	const GAnimationManagerLoad& in_animationManagerLoad,
	const int in_streamIndex
	)const
{
	//GASSERT(false == GetFinished(), "invalid code path");
	if (!mAnimationInstance->HasStream(in_streamIndex))
	{
		return;
	}

	if (1.0F == mWeight)
	{
		mAnimationInstance->ApplyToStreamRotation(
			inout_at,
			inout_up,
			in_animationManagerLoad,
			in_streamIndex
			);
	}
	else
	{
		GVector3Float valueAt;
		GVector3Float valueUp;
		mAnimationInstance->ApplyToStreamRotation(
			valueAt,
			valueUp,
			in_animationManagerLoad,
			in_streamIndex
			);

		inout_at = Lerp(mWeight, inout_at, valueAt);
		inout_up = Lerp(mWeight, inout_up, valueUp);
	}

	return;
}

void GAnimationWrapper::ApplyToStreamFloat(
	float& inout_scale,
	const GAnimationManagerLoad& in_animationManagerLoad,
	const int in_streamIndex,
	const float in_default
	)const
{
	//GASSERT(false == GetFinished(), "invalid code path");
	if (!mAnimationInstance->HasStream(in_streamIndex))
	{
		return;
	}

	if (1.0F == mWeight)
	{
		mAnimationInstance->ApplyToStreamFloat(
			inout_scale,
			in_animationManagerLoad,
			in_streamIndex
			);
	}
	else
	{
		float value = in_default;
		mAnimationInstance->ApplyToStreamFloat(
			value,
			in_animationManagerLoad,
			in_streamIndex
			);

		inout_scale = GMath::Lerp(mWeight, inout_scale, value);
	}

	return;
}

void GAnimationWrapper::FadeOut(const float in_fadeOutDuration)
{
	if (0.0F == in_fadeOutDuration)
	{
		Stop();
	}
	else
	{
		float ratio = 1.0F;
		if (0 != (mFlag & TAnimationWrapperFlag::TFadeIn))
		{
			ratio = GMath::Clamp(GMath::GetRatio(mFadeTime, 0.0F, mFadeDuration), 0.0F, 1.0F);
		}
		mFadeTime = GMath::Lerp(1.0F - ratio, 0.0F, in_fadeOutDuration);
		mFadeDuration = in_fadeOutDuration;

		mFlag &= ~TAnimationWrapperFlag::TFadeIn;
		mFlag |= TAnimationWrapperFlag::TFadeOut;
	}
	return;
}

void GAnimationWrapper::Stop()
{
	//clear all other flags on stop
	mFlag = TAnimationWrapperFlag::TFinished;
	return;
}

//public accessors
const bool GAnimationWrapper::GetLoop()const
{
	return mAnimationInstance->GetLoop();
}

//private methods
void GAnimationWrapper::TickFade(const float in_timeDelta)
{
	if (0 != (mFlag & TAnimationWrapperFlag::TManualFade))
	{
		//maual fade bail
		return;
	}

	if (0 == (mFlag & (TAnimationWrapperFlag::TFadeIn | TAnimationWrapperFlag::TFadeOut)))
	{
		//no fade bail
		return;
	}

	mFadeTime += in_timeDelta * mFadeRate;
	
	if (mFadeDuration < mFadeTime)
	{
		if (0 != (mFlag & TAnimationWrapperFlag::TFadeIn))
		{
			//finish fade in
			mWeight = 1.0F;
			mFlag &= ~TAnimationWrapperFlag::TFadeIn;
		}
		else
		{
			//finish fade out
			Stop();
		}
	}
	else
	{
		if (0 != (mFlag & TAnimationWrapperFlag::TFadeIn))
		{
			mWeight = mFadeTime / mFadeDuration;
		}
		else
		{
			mWeight = 1.0F - (mFadeTime / mFadeDuration);
		}
	}

	return;
}

void GAnimationWrapper::TickAnimation(const float in_timeDelta, const GAnimationManagerLoad& in_animationManagerLoad)
{
	if (0 != (mFlag & TAnimationWrapperFlag::TManualTick))
	{
		//maual tick bail
		return;
	}
	
	//stop the animation 
	if (!mAnimationInstance->SimpleTick(in_timeDelta * mTickRate, in_animationManagerLoad))
	{
		Stop();
	}

	return;
}
