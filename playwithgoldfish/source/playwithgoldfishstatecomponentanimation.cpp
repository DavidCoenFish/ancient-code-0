//
//  PlayWithGoldfishStateComponentAnimation.cpp
//  playwithgoldfish
//
//  Created by David Coen on 2011 01 13
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "PlayWithGoldfishStateComponentAnimation.h"

#include "PlayWithGoldfishStateComponentAi.h"
#include "PlayWithGoldfishStateSceneComponentFish.h"

#include <GAnimationManagerInstance.h>
#include <GDictionary.h>

//constructor
PlayWithGoldfishStateComponentAnimation::PlayWithGoldfishStateComponentAnimation(
	TPointerAnimationManagerInstance& inout_animationInstance
	)
	: mAnimationInstance(inout_animationInstance)
	, mCurrentAnimation()
	, mRotationCountdown(0.0F)
	, mMove(false)
	, mFinishedMoveRotate(false)
	, mCurrentAnimationTime(0.0F)
{
	return;
}

PlayWithGoldfishStateComponentAnimation::~PlayWithGoldfishStateComponentAnimation()
{
	return;
}

//public methods
void PlayWithGoldfishStateComponentAnimation::Tick(
	const float in_timeDelta,
	const PlayWithGoldfishStateType::TAnimationType::TEnum in_type,
	GDictionary& inout_dictionary
	)
{
	switch (in_type)
	{
	default:
	case PlayWithGoldfishStateType::TAnimationType::TNone:
		SetAnimation("");
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAI:
		TickAi(in_timeDelta, inout_dictionary);
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAnimationFeed:
		SetAnimation("feed");
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAnimationIdle:
		SetAnimation("idle");
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAnimationIdleAgitated:
		SetAnimation("idle_agitated");
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAnimationIdleSleep:
		SetAnimation("idle_sleepy");
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAnimationMove:
		SetAnimation("move");
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAnimationTurnLeft:
		SetAnimation("turn_left");
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAnimationTurnLeftHard:
		SetAnimation("turn_left_hard");
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAnimationTurnRight:			
		SetAnimation("turn_right");
		break;
	case PlayWithGoldfishStateType::TAnimationType::TAnimationTurnRightHard:
		SetAnimation("turn_right_hard");
		break;
	}
	return;
}

//private methods
void PlayWithGoldfishStateComponentAnimation::SetAnimation(const std::string& in_animationName, const float in_fadeTime)
{
	if (mCurrentAnimation == in_animationName)
	{
		return;
	}

	mCurrentAnimationTime = 0.0F; 
	mCurrentAnimation = in_animationName;
	TPointerAnimationManagerInstance animationManagerInstance = mAnimationInstance.lock();

	if (animationManagerInstance)
	{
		animationManagerInstance->StopAnimation(in_fadeTime);
		animationManagerInstance->PlayAnimation(
			mCurrentAnimation,
			0.0F,
			1.0F,
			1.0F,
			1.0F,
			in_fadeTime
			);
	}

	return;
}

void PlayWithGoldfishStateComponentAnimation::TickAi(
	const float in_timeDelta,
	GDictionary& inout_dictionary
	)
{
	const PlayWithGoldfishStateType::TAiState::TEnum aiState = PlayWithGoldfishStateComponentAi::GetAiState(inout_dictionary);
	if (0.0F < mRotationCountdown)
	{
		mRotationCountdown -= in_timeDelta;
		if (mRotationCountdown <= 0.0F)
		{
			mRotationCountdown = 0.0F;
			if (PlayWithGoldfishStateType::TAiState::TMove == aiState)
			{
				mFinishedMoveRotate = true;
			}
		}
		else
		{
			//set appropriate rotation anim
		}
	}

	mCurrentAnimationTime += in_timeDelta;
	if (mCurrentAnimationTime < 0.1F)
	{
		//help ai not swap states continuiously on edge conditions, means we have min time for rotate though
		return;
	}

	switch (aiState)
	{
	default:
		break;
	case PlayWithGoldfishStateType::TAiState::TMove:
		{
			//have we just entered move
			if (!mMove)
			{
				mFinishedMoveRotate = false;
				mMove = true;

				//start a rotate
				mRotationCountdown = 1.0F;
				const float rotationAmount = PlayWithGoldfishStateComponentAi::GetRotationAmount(inout_dictionary);
				if (rotationAmount < -1.57F)
				{
					SetAnimation("turn_left_hard");
				}
				else if (rotationAmount < 0.0F)
				{
					SetAnimation("turn_left");
				}
				else if (rotationAmount < 1.57F)
				{
					SetAnimation("turn_right");
				}
				else
				{
					SetAnimation("turn_right_hard");
				}
			}

			if (mFinishedMoveRotate)
			{
				SetAnimation("move");
			}
		}
		break;
	case PlayWithGoldfishStateType::TAiState::TIdle:
		{
			mMove = false;
			const float excitment = PlayWithGoldfishStateComponentAi::GetExcitmentLevel(inout_dictionary);
			const float timeSinceLastAction = PlayWithGoldfishStateComponentAi::GetTimeSinceLastAction(inout_dictionary);

			if (20.0F < timeSinceLastAction)
			{
				SetAnimation("idle_sleepy", 2.0F);
			}
			else if (0.25F < excitment)
			{
				const float distanceTouch = LengthAprox(
					PlayWithGoldfishStateSceneComponentFish::GetPosition(inout_dictionary) - 
					PlayWithGoldfishStateSceneComponentFish::GetTouchWorldPoint(inout_dictionary)
					);
				if (distanceTouch < 62.5F)
				{
					SetAnimation("feed");
				}
				else
				{
					SetAnimation("idle_agitated");
				}
			}
			else
			{
				SetAnimation("idle");
			}
		}
		break;
	}

	return;
}
