//
//  PlayWithGoldfishStateComponentAi.h
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _PlayWithGoldfishStateComponentAi_h_
#define _PlayWithGoldfishStateComponentAi_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <GVector3Float.h>
#include "PlayWithGoldfishStateType.h"


class GAnimationManagerInstance;
class GDictionary;

/*
	ideal is to have ai oblivious to animation, other than exposing variables
*/
class PlayWithGoldfishStateComponentAi : public boost::noncopyable
{
	//typedef
private:

	//static public methods
public:
	static const float GetTimeSinceTouchActive(GDictionary& inout_dictionary);
	static void SetTimeSinceTouchActive(GDictionary& inout_dictionary, const float in_value);

	static const float GetExcitmentLevel(GDictionary& inout_dictionary);
	static void SetExcitmentLevel(GDictionary& inout_dictionary, const float in_value);

	static const PlayWithGoldfishStateType::TAiState::TEnum GetAiState(GDictionary& inout_dictionary);
	static void SetAiState(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TAiState::TEnum in_value);

	static const float GetRotationAmount(GDictionary& inout_dictionary);
	static void SetRotationAmount(GDictionary& inout_dictionary, const float in_value);

	static const float GetTimeSinceLastAction(GDictionary& inout_dictionary);
	static void SetTimeSinceLastAction(GDictionary& inout_dictionary, const float in_value);

	//constructor
public:
	PlayWithGoldfishStateComponentAi();
	~PlayWithGoldfishStateComponentAi();

	//public methods
public:
	void Tick(
		const float in_timeDelta,
		GDictionary& inout_dictionary
		);
	void ShakeWindow(GDictionary& inout_dictionary);

	//private members
private:
	float mStateTime;

	GVector3Float mMoveFrom;
	GVector3Float mMoveTo;
	float mMoveDuration;
	float mFacingFrom;
	float mFacingTo;
	float mFacingDuration;

	GVector3Float mIdleDriftVelocity;
	float mIdleFacingVelocity;

};

#endif //_PlayWithGoldfishStateComponentAi_h_