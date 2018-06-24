//
//  GAnimationPlayParam.h
//
//  Created by David Coen on 2011 02 06
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationPlayParam_h_
#define _GAnimationPlayParam_h_

#include <boost/noncopyable.hpp>

class GAnimationPlayParam : public boost::noncopyable
{
	//constructor
public:
	GAnimationPlayParam(
		const float in_tickRate = 1.0F,
		const float in_fadeInDuration = 0.16666F,
		const float in_startTime = 0.0F,
		const float in_weight = 1.0F,
		const float in_fadeRate = 1.0F,
		const bool in_manualTick = false,
		const bool in_manualFade = false
		);
	~GAnimationPlayParam();

	//public accessors
public:
	const float GetStartTime()const{ return mStartTime; }
	const float GetWeight()const{ return mWeight; }
	const float GetTickRate()const{ return mTickRate; }
	const float GetFadeRate()const{ return mFadeRate; }
	const float GetFadeInDuration()const{ return mFadeInDuration; }
	const bool GetManualTick()const{ return mManualTick; }
	const bool GetManualFade()const{ return mManualFade; }

	//private members;
private:
	const float mStartTime;
	const float mWeight;
	const float mTickRate;
	const float mFadeRate;
	const float mFadeInDuration;
	const bool mManualTick;
	const bool mManualFade;

};

#endif 