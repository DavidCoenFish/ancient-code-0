//
//  GAnimationPlayParam.cpp
//
//  Created by David Coen on 2011 02 06
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationPlayParam.h"

//constructor
GAnimationPlayParam::GAnimationPlayParam(
	const float in_tickRate,
	const float in_fadeInDuration,
	const float in_startTime,
	const float in_weight,
	const float in_fadeRate,
	const bool in_manualTick,
	const bool in_manualFade
	)
	: mStartTime(in_startTime)
	, mWeight(in_weight)
	, mTickRate(in_tickRate)
	, mFadeRate(in_fadeRate)
	, mFadeInDuration(in_fadeInDuration)
	, mManualTick(in_manualTick)
	, mManualFade(in_manualFade)
{
	return;
}

GAnimationPlayParam::~GAnimationPlayParam()
{
	return;
}
