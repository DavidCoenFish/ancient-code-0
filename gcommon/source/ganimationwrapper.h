//
//  GAnimationWrapper.h
//
//  Created by David Coen on 2011 01 06
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationWrapper_h_
#define _GAnimationWrapper_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

class GAnimationInstance;
class GAnimationManagerLoad;
class GVector3Float;

class GAnimationWrapper : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GAnimationInstance> TPointerAnimationInstance;
	typedef boost::weak_ptr<GAnimationInstance> TWeakAnimationInstance;

	//constructor
public:
	GAnimationWrapper(
		TPointerAnimationInstance& inout_animationInstance, //WARN transfer ownership
		const float in_weight = 1.0F,
		const float in_tickRate = 1.0F,
		const float in_fadeRate = 1.0F,
		const float in_fadeInDuration = 0.0F,
		const bool in_manualTick = false,
		const bool in_manualFade = false
		);
	~GAnimationWrapper();

	//public methods
public:
	//non looping past end or fade out finished
	const bool GetFinished()const;

	//return false if non looping animation has past end or beginning. delta should be positive, for backwards, change rate
	//if update is manualTick, only fade is ticked. 
	const bool SimpleTick(const float in_timeDelta, const GAnimationManagerLoad& in_animationManagerLoad);

	void ApplyToStreamPosition(
		GVector3Float& inout_position,
		const GAnimationManagerLoad& in_animationManagerLoad,
		const int in_streamIndex
		)const;
	void ApplyToStreamRotation(
		GVector3Float& inout_at,
		GVector3Float& inout_up,
		const GAnimationManagerLoad& in_animationManagerLoad,
		const int in_streamIndex
		)const;
	void ApplyToStreamFloat(
		float& inout_scale,
		const GAnimationManagerLoad& in_animationManagerLoad,
		const int in_streamIndex,
		const float in_default
		)const;

	//void TriggerNoteTrack(const float in_lowTime, const float in_highTime, TriggerNoteCallback); 

	//if we are only %30 faded in, we advance to %70 faded out
	void FadeOut(const float in_fadeOutDuration);
	void Stop(); //same as calling FadeOut(0.0F);

	//public accessors
public:
	const float GetWeight()const{ return mWeight; }
	void SetWeight(const float in_weight){ mWeight = in_weight; return; }

	const float GetTickRate()const{ return mTickRate; }
	void SetTickRate(const float in_tickRate){ mTickRate = in_tickRate; return; }

	const float GetFadeRate()const{ return mFadeRate; }
	void SetFadeRate(const float in_fadeRate){ mFadeRate = in_fadeRate; return; }

	const bool GetLoop()const;

	//private methods
private:
	void TickFade(const float in_timeDelta);
	void TickAnimation(const float in_timeDelta, const GAnimationManagerLoad& in_animationManagerLoad);

	//private members;
private:
	TPointerAnimationInstance mAnimationInstance;
	float mFadeDuration;
	float mFadeTime;
	float mWeight;
	float mTickRate;
	float mFadeRate;
	int mFlag; //manual, fadein, fadeout

};

#endif 