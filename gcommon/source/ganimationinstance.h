//
//  GAnimationInstance.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationInstance_h_
#define _GAnimationInstance_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GVector3Float.h"

class GAsset;
class GAnimationManagerLoad;

class GAnimationInstance : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GAsset> TPointerAsset;
	typedef boost::weak_ptr<GAsset> TWeakAsset;

	typedef std::vector<int> TArrayInt;

	//constructor
public:
	GAnimationInstance(
		const GAnimationManagerLoad& in_animationManagerLoad,
		const int in_animationLoadIndex,
		const float in_timePosition = 0.0F //,
		//const float in_weight = 1.0F
		);
	~GAnimationInstance();

	//public methods
public:
	//return false if non looping animation has past end or beginning. delta can be pos or neg
	const bool SimpleTick(const float in_timeDelta, const GAnimationManagerLoad& in_animationManagerLoad);

	const bool HasStream(const int in_streamIndex)const;

	void ApplyToStreamPosition(
		GVector3Float& out_position,
		const GAnimationManagerLoad& in_animationManagerLoad,
		const int in_streamIndex
		)const;
	void ApplyToStreamRotation(
		GVector3Float& out_at,
		GVector3Float& out_up,
		const GAnimationManagerLoad& in_animationManagerLoad,
		const int in_streamIndex
		)const;
	void ApplyToStreamFloat(
		float& out_scale,
		const GAnimationManagerLoad& in_animationManagerLoad,
		const int in_streamIndex
		)const;

	//void TriggerNoteTrack(const float in_lowTime, const float in_highTime, TriggerNoteCallback); 

	//public accessors
public:
	void SetTimePosition(
		const GAnimationManagerLoad& in_animationManagerLoad,
		const float in_timePosition, 
		const bool in_resetHintBegining, //passed end and looping frm begining
		const bool in_resetHintEnd //going backwards and passed beginnning and back at end
		);
	const float GetTimePosition()const{ return mTimePosition; }

	//void SetWeight(const float in_weight){ mWeight = in_weight; return; }
	//const float GetWeight()const{ return mWeight; }

	const bool GetLoop()const{ return m_loop; }

	//private members;
private:
	const int mAnimationLoadIndex;
	TArrayInt mArrayStreamKeyIndexHint; //for each stream, keep the last key index position, as starting hint for next data search
	float mTimePosition;
	//float mWeight; do weights upstream

	//well, either need reference to in_animationManagerLoad or hold some state
	const bool m_loop;

};

#endif 