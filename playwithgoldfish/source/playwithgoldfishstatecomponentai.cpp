//
//  PlayWithGoldfishStateComponentAi.cpp
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "PlayWithGoldfishStateComponentAi.h"

#include "PlayWithGoldfishStateSceneComponentFish.h"

#include <GDictionary.h>
#include <GMath.h>

static const char* const sTimeSinceTouchActive = "aiTimeSinceTouchActive";
static const char* const sExcitmentLevel = "aiExcitmentLevel";
static const char* const sAiState = "aiState";
static const char* const sRotationAmount = "aiRotationAmount";
static const char* const sTimeSinceLastAction = "aiTimeSinceLastAction";

static void LocalDoctorVelocity(
	float& inout_velocity,
	const float in_targetVelocity,
	const float in_rateChange,
	const float in_timeDelta
	)
{
	if (inout_velocity < in_targetVelocity)
	{
		inout_velocity = std::min(in_targetVelocity, inout_velocity + (in_rateChange * in_timeDelta));
	}
	else
	{
		inout_velocity = std::max(in_targetVelocity, inout_velocity - (in_rateChange * in_timeDelta));
	}

	return;
}

static void LocalExcitedTrackTouch(
	const float in_excitmentLevel,
	GVector3Float&inout_idleDriftVelocity,
	float& inout_idleFacingVelocity,
	GDictionary& inout_dictionary,
	const float in_timeDelta
	)
{
	if (in_excitmentLevel < 0.5F)
	{
		if (0.05F < GMath::Abs(inout_idleFacingVelocity))
		{
			LocalDoctorVelocity(inout_idleFacingVelocity, 0.0F, 0.1F, in_timeDelta);
		}

		if (0.5F < LengthSquared(inout_idleDriftVelocity))
		{
			LocalDoctorVelocity(inout_idleDriftVelocity[0], 0.0F, 1.0F, in_timeDelta);
			LocalDoctorVelocity(inout_idleDriftVelocity[1], 0.0F, 1.0F, in_timeDelta);
			LocalDoctorVelocity(inout_idleDriftVelocity[2], 0.0F, 1.0F, in_timeDelta);
		}

		return;
	}

	const GVector3Float position = PlayWithGoldfishStateSceneComponentFish::GetPosition(inout_dictionary);
	const GVector3Float touchPoint = PlayWithGoldfishStateSceneComponentFish::GetTouchWorldPoint(inout_dictionary) - GVector3Float(0.0F, 40.0F, 0.0F);
	float length = 0.0F;
	const GVector3Float normal = NormaliseAprox(touchPoint - position, length);

	//inout_idleDriftVelocity = normal *25.0F;
	const GVector3Float targetVelocity = normal * GMath::Clamp(length * 0.5F, 15.0F, 50.0F);

	LocalDoctorVelocity(inout_idleDriftVelocity[0], targetVelocity[0], 15.0F, in_timeDelta);
	LocalDoctorVelocity(inout_idleDriftVelocity[1], targetVelocity[1], 15.0F, in_timeDelta);
	LocalDoctorVelocity(inout_idleDriftVelocity[2], targetVelocity[2], 15.0F, in_timeDelta);

	const float facing = GMath::ClampRadian(PlayWithGoldfishStateSceneComponentFish::GetFacing(inout_dictionary));
	const float targetRadian = GMath::ClampRadian(GMath::ATan(normal.GetX(), normal.GetZ()));
	const float yawOffsetRadian = GMath::ClampRadian(targetRadian - facing);
	//inout_idleFacingVelocity = yawOffsetRadian;
			
	LocalDoctorVelocity(inout_idleFacingVelocity, yawOffsetRadian, 0.5F, in_timeDelta);

	return;
}

//static public methods
/*static*/ const float PlayWithGoldfishStateComponentAi::GetTimeSinceTouchActive(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueFloat(sTimeSinceTouchActive);
}

/*static*/ void PlayWithGoldfishStateComponentAi::SetTimeSinceTouchActive(GDictionary& inout_dictionary, const float in_value)
{
	inout_dictionary.SetValueFloat(sTimeSinceTouchActive, in_value);
	return;
}

/*static*/ const float PlayWithGoldfishStateComponentAi::GetExcitmentLevel(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueFloat(sExcitmentLevel);
}

/*static*/ void PlayWithGoldfishStateComponentAi::SetExcitmentLevel(GDictionary& inout_dictionary, const float in_value)
{
	inout_dictionary.SetValueFloat(sExcitmentLevel, in_value);
	return;
}

/*static*/ const PlayWithGoldfishStateType::TAiState::TEnum PlayWithGoldfishStateComponentAi::GetAiState(GDictionary& inout_dictionary)
{
	return (PlayWithGoldfishStateType::TAiState::TEnum)inout_dictionary.GetValueInt(sAiState);
}

/*static*/ void PlayWithGoldfishStateComponentAi::SetAiState(GDictionary& inout_dictionary, const PlayWithGoldfishStateType::TAiState::TEnum in_value)
{
	inout_dictionary.SetValueInt(sAiState, in_value);
	return;
}

/*static*/ const float PlayWithGoldfishStateComponentAi::GetRotationAmount(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueFloat(sRotationAmount);
}

/*static*/ void PlayWithGoldfishStateComponentAi::SetRotationAmount(GDictionary& inout_dictionary, const float in_value)
{
	inout_dictionary.SetValueFloat(sRotationAmount, in_value);
	return;
}
/*static*/ const float PlayWithGoldfishStateComponentAi::GetTimeSinceLastAction(GDictionary& inout_dictionary)
{
	return inout_dictionary.GetValueFloat(sTimeSinceLastAction);
}
/*static*/ void PlayWithGoldfishStateComponentAi::SetTimeSinceLastAction(GDictionary& inout_dictionary, const float in_value)
{
	inout_dictionary.SetValueFloat(sTimeSinceLastAction, in_value);
	return;
}

//constructor
PlayWithGoldfishStateComponentAi::PlayWithGoldfishStateComponentAi()
: mStateTime(0.0F)
, mMoveFrom()
, mMoveTo()
, mMoveDuration(0.0F)
, mFacingFrom(0.0F)
, mFacingTo(0.0F)
, mFacingDuration(0.0F)
, mIdleDriftVelocity()
, mIdleFacingVelocity(0.0F)
{
	return;
}

PlayWithGoldfishStateComponentAi::~PlayWithGoldfishStateComponentAi()
{
	return;
}

//public methods
void PlayWithGoldfishStateComponentAi::Tick(
	const float in_timeDelta,
	GDictionary& inout_dictionary
	)
{
	//update variables
	const bool touchBegin = PlayWithGoldfishStateSceneComponentFish::GetTouchBegin(inout_dictionary);
	const bool touchActive = PlayWithGoldfishStateSceneComponentFish::GetTouchActive(inout_dictionary);
	float timeSinceTouchActive = GetTimeSinceTouchActive(inout_dictionary);
	float excitmentLevel = GetExcitmentLevel(inout_dictionary);
	float timeSinceLastAction = GetTimeSinceLastAction(inout_dictionary);

	timeSinceLastAction += in_timeDelta;

	if (touchActive)
	{
		timeSinceLastAction = 0.0F;
		timeSinceTouchActive = 0.0F;
	}
	else
	{
		timeSinceTouchActive += in_timeDelta;
	}

	if (touchBegin)
	{
		excitmentLevel += 0.333F;
	}
	if (touchActive)
	{
		excitmentLevel += (0.25F * in_timeDelta);
	}
	else
	{
		excitmentLevel -= (0.125F * in_timeDelta);
	}
	excitmentLevel = GMath::Clamp(excitmentLevel, 0.0F, 1.0F);

	
	SetTimeSinceTouchActive(inout_dictionary, timeSinceTouchActive);
	SetExcitmentLevel(inout_dictionary, excitmentLevel);

	mStateTime += in_timeDelta;

	//consider state change
	PlayWithGoldfishStateType::TAiState::TEnum state = GetAiState(inout_dictionary);
	if (state == PlayWithGoldfishStateType::TAiState::TIdle)
	{
		bool doMove = false;
		if (1.0F < timeSinceTouchActive)
		{
			doMove = GMath::RandomUnit() < (std::max(0.05F, (1.0F / timeSinceTouchActive)) * in_timeDelta);
		}

		if (doMove)
		{
			timeSinceLastAction = 0.0F;
			state = PlayWithGoldfishStateType::TAiState::TMove;
			mStateTime = 0.0F;
			mMoveFrom = PlayWithGoldfishStateSceneComponentFish::GetPosition(inout_dictionary);
			mMoveTo = PlayWithGoldfishStateSceneComponentFish::GetCameraTarget(inout_dictionary) +
				GVector3Float(100.0F * GMath::RandomUnitPlusMinius(), 50.0F * GMath::RandomUnitPlusMinius(), 100.0F * GMath::RandomUnitPlusMinius());
			mFacingFrom = PlayWithGoldfishStateSceneComponentFish::GetFacing(inout_dictionary);
			const GVector3Float offset = mMoveTo - mMoveFrom;
			mFacingTo = GMath::ATan(offset.GetX(), offset.GetZ());
			mMoveDuration = std::max(1.0F, (LengthAprox(mMoveTo - mMoveFrom) * 0.025F) - excitmentLevel);
			mFacingDuration = 1.0F;
		}
	}
	else if (state == PlayWithGoldfishStateType::TAiState::TMove)
	{
		if (mMoveDuration < mStateTime)
		{
			//finish move
			state = PlayWithGoldfishStateType::TAiState::TIdle;
			mStateTime = 0.0F;
			PlayWithGoldfishStateSceneComponentFish::SetPosition(inout_dictionary, mMoveTo);
			PlayWithGoldfishStateSceneComponentFish::SetFacing(inout_dictionary, mFacingTo);
			mIdleDriftVelocity = (mMoveTo - mMoveFrom) * (0.5F /  mMoveDuration);
		}
		else if (touchActive)
		{
			//intrupt move to chase touch
			float moveRateOfChange = 0.0F;
			if (0.0F != mMoveDuration)
			{
				moveRateOfChange = GMath::RatioEaseInOutRateOfChange(GMath::Clamp(mStateTime / mMoveDuration, 0.0F, 1.0F));

				mIdleDriftVelocity = ((mMoveTo - mMoveFrom) / mMoveDuration) * moveRateOfChange;
			}
			else
			{
				mIdleDriftVelocity = GVector3Float::sZero;
			}

			state = PlayWithGoldfishStateType::TAiState::TIdle;
			mStateTime = 0.0F;
		}
	}
	SetAiState(inout_dictionary, state);


	//update position, facing
	GVector3Float position = PlayWithGoldfishStateSceneComponentFish::GetPosition(inout_dictionary);
	float facing = PlayWithGoldfishStateSceneComponentFish::GetFacing(inout_dictionary);
	switch (state)
	{
	default:
		break;
	case PlayWithGoldfishStateType::TAiState::TIdle:
		LocalExcitedTrackTouch(
			excitmentLevel,
			mIdleDriftVelocity,
			mIdleFacingVelocity,
			inout_dictionary,
			in_timeDelta
			);
		position += (mIdleDriftVelocity * in_timeDelta);
		facing += (mIdleFacingVelocity * in_timeDelta);
		break;
	case PlayWithGoldfishStateType::TAiState::TMove:
		{
			float moveRatio = 1.0F;
			if (0.0F != mMoveDuration)
			{
				moveRatio = GMath::RatioEaseInOut(GMath::Clamp(mStateTime / mMoveDuration, 0.0F, 1.0F));
			}
			position = Lerp(moveRatio, mMoveFrom, mMoveTo);

			float rotateRatio = 1.0F;
			if (0.0F != mFacingDuration)
			{
				rotateRatio = GMath::RatioEaseInOut(GMath::Clamp(mStateTime / mFacingDuration, 0.0F, 1.0F));
			}
			facing = mFacingFrom + GMath::Lerp(rotateRatio, 0.0F, GMath::ClampRadian(mFacingTo - mFacingFrom));
			//facing = GMath::ClampRadian(GMath::Lerp(rotateRatio, GMath::ClampRadian(mFacingFrom), GMath::ClampRadian(mFacingTo)));
		}
		break;
	}

	PlayWithGoldfishStateSceneComponentFish::SetPosition(inout_dictionary, position);
	PlayWithGoldfishStateSceneComponentFish::SetFacing(inout_dictionary, facing);

	SetTimeSinceLastAction(inout_dictionary, timeSinceLastAction);

	return;
}

void PlayWithGoldfishStateComponentAi::ShakeWindow(GDictionary& inout_dictionary)
{
	mStateTime = 0.0F;
	mMoveFrom = GVector3Float::sZero;
	mMoveTo = GVector3Float::sZero;
	mMoveDuration = 0.0F;
	mFacingFrom = 0.0F;
	mFacingTo = 0.0F;
	mFacingDuration = 0.0F;
	mIdleDriftVelocity = GVector3Float::sZero;
	mIdleFacingVelocity = 0.0F;
	
	SetTimeSinceTouchActive(inout_dictionary, 0.0F);
	SetExcitmentLevel(inout_dictionary, 0.0F);
	SetAiState(inout_dictionary, PlayWithGoldfishStateType::TAiState::TIdle);
	SetRotationAmount(inout_dictionary, 0.0F);
	SetTimeSinceLastAction(inout_dictionary, 0.0F);	
	
	return;
}
