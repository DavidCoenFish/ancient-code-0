//
//  ZombieCameraDeath.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 25
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieCameraDeath.h"

#include <GMath.h>
#include <GInput.h>
#include <GInputJoystick.h>
#include <GVector3Float.h>
#include <GMatrix9Float.h>
#include <GComponentLerpVector3Float.h>

//constructor
ZombieCameraDeath::ZombieCameraDeath()
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

ZombieCameraDeath::~ZombieCameraDeath()
{
	return;
}

//public methods
void ZombieCameraDeath::Tick(const float in_timeDelta)
{
	mLerpPosition->Tick(in_timeDelta);
	mLerpAt->Tick(in_timeDelta);
	mLerpUp->Tick(in_timeDelta);

	return;
}

void ZombieCameraDeath::Input(
	const GInput& in_input, 
	const GVector3Float& in_joystickDirection
	)
{
	mRoll = GMath::ATan(in_joystickDirection.m_x, -in_joystickDirection.m_y);

	return;
}

//public accessors
void ZombieCameraDeath::Activate(
	const GVector3Float& in_position,
	const GVector3Float& in_at, 
	const GVector3Float& in_up
	)
{
	mLerpPosition->SetValue(in_position);
	mLerpPosition->SetValue(GVector3Float(in_position.m_x, 0.1F, in_position.m_z), 5.0F, GComponentLerp::TType::TEaseInOut);

	mLerpAt->SetValue(in_at);
	mLerpAt->SetValue(-GVector3Float::sUnitY, 5.0F, GComponentLerp::TType::TEaseInOut);
	mLerpUp->SetValue(in_up);
	mLerpUp->SetValue(in_at, 5.0F, GComponentLerp::TType::TEaseInOut);

	return;
}

void ZombieCameraDeath::GetData(
	GVector3Float& out_position,
	GVector3Float& out_at,
	GVector3Float& out_up
	)
{
	out_position = mLerpPosition->GetValue();
	out_at = mLerpAt->GetValue();
	out_up = mLerpUp->GetValue();

	const GMatrix9Float rollTransform = GMatrix9FloatConstructAxisAngle(out_at, mRoll);
	out_up = out_up * rollTransform;

	return;
}