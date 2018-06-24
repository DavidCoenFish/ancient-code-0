//
//  ZombieReloadDetector.cpp
//  Zombie
//
//  Created by David Coen on 2011 02 01
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieReloadDetector.h"

#include <GInput.h>
#include <GInputJoystick.h>

//DEBUG
#include "applicationtaskgame.h"
#include <GStringUtility.h>

static const float sZombieReloadDetectorSmooth = 0.1F;
static const float sZombieReloadDetectorCooldown = 0.1F;
static const float sZombieReloadDetectorTrigger = 0.3F;

//constructor
ZombieReloadDetector::ZombieReloadDetector(const GVector3Float& in_accelerometorSeed)
	: mTriggerReload(false)
	, mValid(false)
	, mCoolDown(0.0F)
	, mAccelerometorAverage(in_accelerometorSeed)
{
	return;
}

ZombieReloadDetector::~ZombieReloadDetector()
{
	return;
}

//public methods
void ZombieReloadDetector::Input(const GInput& in_input)
{
	mTriggerReload = false;
	GVector3Float accelerometer(-GVector3Float::sUnitY);
	if (1 <= in_input.GetJoystickCount())
	{
		accelerometer = in_input.GetJoystick(0).GetDirection();

		if (!mValid)
		{
			mValid = true;
			mAccelerometorAverage = accelerometer;
			return;
		}
	}
	
	if (!mValid)
	{
		return;
	}

	mAccelerometorAverage = (mAccelerometorAverage * (1.0F - sZombieReloadDetectorSmooth)) + (accelerometer * sZombieReloadDetectorSmooth);

	const float delta = 1.0F - DotProduct(NormaliseAprox(mAccelerometorAverage), NormaliseAprox(accelerometer));

//#ifdef DSC_DEBUG
//	{
//		std::string text("delta: ");
//		text += GStringUtility::Print(delta);
//		DEBUG_PUT_TEXT(text);
//	}
//#endif

	if ((sZombieReloadDetectorTrigger < delta) && (!mCoolDown))
	{
		mCoolDown = sZombieReloadDetectorCooldown;
		mTriggerReload = true;
	}

	return;
}

void ZombieReloadDetector::Tick(const float in_timeDelta)
{
	if (mCoolDown)
	{
		mCoolDown -= in_timeDelta;
		if (mCoolDown < 0.0F)
		{
			mCoolDown = 0.0F;
		}
	}

	return;
}
