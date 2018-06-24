//
//  ZombieCameraCinematic.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 12
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieCameraCinematic.h"

#include <GMath.h>
#include <GInput.h>
#include <GInputJoystick.h>
#include <GVector3Float.h>
#include <GMatrix9Float.h>
#include <GComponentLerpVector3Float.h>

//constructor
ZombieCameraCinematic::ZombieCameraCinematic()
: mLerpPosition()
, mLerpAt()
, mLerpUp()
, mRoll(0.0F)
{
	mLerpPosition.reset(new GComponentLerpVector3Float);
	mLerpAt.reset(new GComponentLerpVector3Float(-GVector3Float::sUnitZ));
	mLerpUp.reset(new GComponentLerpVector3Float(GVector3Float::sUnitY));
	return;
}

ZombieCameraCinematic::~ZombieCameraCinematic()
{
	return;
}

//public methods
void ZombieCameraCinematic::Tick(const float in_timeDelta)
{
	mLerpPosition->Tick(in_timeDelta);
	mLerpAt->Tick(in_timeDelta);
	mLerpUp->Tick(in_timeDelta);

	return;
}

void ZombieCameraCinematic::Input(
	const GInput& in_input, 
	const GVector3Float& in_joystickDirection
	)
{
	mRoll = GMath::ATan(in_joystickDirection.m_x, -in_joystickDirection.m_y);

	return;
}

//public accessors
void ZombieCameraCinematic::SetPosition(
	const GVector3Float& in_position, 
	const float in_lerpDuration,
	const GComponentLerp::TType::TEnum in_lerpType
	)
{
	mLerpPosition->SetValue(in_position, in_lerpDuration, in_lerpType);
	return;
}

void ZombieCameraCinematic::SetRotation(
	const GVector3Float& in_at, 
	const GVector3Float& in_up, 
	const float in_lerpDuration,
	const GComponentLerp::TType::TEnum in_lerpType
	)
{
	mLerpAt->SetValue(in_at, in_lerpDuration, in_lerpType);
	mLerpUp->SetValue(in_up, in_lerpDuration, in_lerpType);
	return;
}

void ZombieCameraCinematic::GetData(
	GVector3Float& out_position,
	GVector3Float& out_at,
	GVector3Float& out_up,
	const bool in_useRoll
	)
{
	out_position = mLerpPosition->GetValue();
	out_at = mLerpAt->GetValue();
	out_up = mLerpUp->GetValue();

	if (in_useRoll)
	{
		const GMatrix9Float rollTransform = GMatrix9FloatConstructAxisAngle(out_at, mRoll);
		out_up = out_up * rollTransform;
	}

	return;
}