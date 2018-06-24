//
//  ZombieCameraGame.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 12
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieCameraGame.h"

#include <GInput.h>
#include <GInputJoystick.h>
#include <GVector3Float.h>
#include <GComponentLerpVector3Float.h>
#include <GComponentLerpFloat.h>
#include <GMath.h>
#include <GMatrix9Float.h>

static const float sNudgeViewUp = 0.4F;

//constructor
ZombieCameraGame::ZombieCameraGame()
: mLerpPosition()
, mLerpFacing()
, mRize(0.0F)
, mRoll(0.0F) 
{
	mLerpPosition.reset(new GComponentLerpVector3Float);
	mLerpFacing.reset(new GComponentLerpFloat);
	return;
}

ZombieCameraGame::~ZombieCameraGame()
{
	return;
}

//public methods
void ZombieCameraGame::Tick(const float in_timeDelta)
{
	mLerpPosition->Tick(in_timeDelta);
	mLerpFacing->Tick(in_timeDelta);
	return;
}

void ZombieCameraGame::Input(
	const GInput& in_input, 
	const GVector3Float& in_joystickDirection
	)
{
#if 0
	//limit the rize to a range, and nudge the view angle up a bit
	//mRize = GMath::Clamp(((GMath::ACos(in_joystickDirection.m_z) / GMath::GetHalfPI()) - (1.0F + sNudgeViewUp)), -0.9F, 0.9F) * GMath::GetHalfPI();
	//acos(-1) = pi   acos(0) = pi/2  acos(1) = 0
	float temp = (GMath::ACos(in_joystickDirection.m_z) / GMath::GetPI());
	temp = ((temp * temp * temp) - 0.5F) * 2.0F;
	mRize = temp * GMath::GetHalfPI();

	mRoll = GMath::ATan(in_joystickDirection.m_x, -in_joystickDirection.m_y);
#else
	mRize = 0.0F;
	mRoll = 0.0F;
#endif
	return;
}

//public accessors
void ZombieCameraGame::SetPosition(
	const GVector3Float& in_position, 
	const float in_lerpDuration,
	const GComponentLerp::TType::TEnum in_lerpType
	)
{
	mLerpPosition->SetValue(in_position, in_lerpDuration, in_lerpType);
	return;
}

void ZombieCameraGame::SetRotation(
	const GVector3Float& in_at, 
	const GVector3Float& in_up, 
	const float in_lerpDuration,
	const GComponentLerp::TType::TEnum in_lerpType
	)
{
	const float facing = GMath::ATan(in_at.m_x, in_at.m_z);
	mLerpFacing->SetValueRadian(facing, in_lerpDuration, in_lerpType);

	return;
}

void ZombieCameraGame::GetData(
	GVector3Float& out_position,
	GVector3Float& out_at,
	GVector3Float& out_up
	)
{
	out_position = mLerpPosition->GetValue();

	out_at = GVector3Float::sUnitZ;
	out_up = GVector3Float::sUnitY;

	//todo: concatinate matrix

	const GMatrix9Float rollTransform = GMatrix9FloatConstructAxisAngle(GVector3Float::sUnitZ, mRoll);
	out_up = out_up * rollTransform;

	const GMatrix9Float rizeTransform = GMatrix9FloatConstructAxisAngle(GVector3Float::sUnitX, mRize);
	out_at = out_at * rizeTransform;
	out_up = out_up * rizeTransform;

	const GMatrix9Float facingTransform = GMatrix9FloatConstructAxisAngle(GVector3Float::sUnitY, mLerpFacing->GetValue());
	out_at = out_at * facingTransform;
	out_up = out_up * facingTransform;

	return;
}