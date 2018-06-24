//
//  PlayWithGoldfishStateComponentAnimation.h
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _PlayWithGoldfishStateComponentAnimation_h_
#define _PlayWithGoldfishStateComponentAnimation_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <string>
#include "PlayWithGoldfishStateType.h"


class GAnimationManagerInstance;
class GDictionary;

/*
	match animation to variable state in dictionary (for ai) or PlayWithGoldfishStateType::TAnimationType
*/
class PlayWithGoldfishStateComponentAnimation : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GAnimationManagerInstance> TPointerAnimationManagerInstance;
	typedef boost::weak_ptr<GAnimationManagerInstance> TWeakAnimationManagerInstance;

	//constructor
public:
	PlayWithGoldfishStateComponentAnimation(
		TPointerAnimationManagerInstance& inout_animationInstance
		);
	~PlayWithGoldfishStateComponentAnimation();

	//public methods
public:
	void Tick(
		const float in_timeDelta,
		const PlayWithGoldfishStateType::TAnimationType::TEnum in_type,
		GDictionary& inout_dictionary
		);

	//private methods
private:
	void SetAnimation(const std::string& in_animationName, const float in_fadeTime = 0.5F);

	void TickAi(
		const float in_timeDelta,
		GDictionary& inout_dictionary
		);

	//private members
private:
	TWeakAnimationManagerInstance mAnimationInstance;
	std::string mCurrentAnimation;
	float mRotationCountdown;
	bool mMove;
	bool mFinishedMoveRotate;
	float mCurrentAnimationTime;


};

#endif //_PlayWithGoldfishStateComponentAnimation_h_