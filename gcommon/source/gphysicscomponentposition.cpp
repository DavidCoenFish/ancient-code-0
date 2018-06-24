//
//  GPhysicsComponentPosition.cpp
//
//  Created by David Coen on 2010 10 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GPhysicsComponentPosition.h"
#include "GMath.h"

//static methods
/*static*/ const GVector3Float GPhysicsComponentPosition::CalculateGetVelocity(
	const GVector3Float& in_oldPosition,
	const GVector3Float& in_newPosition,
	const float in_timeDelta
	)
{
	if (GMath::AlmostZero(in_timeDelta))
	{
		return GVector3Float::sZero;
	}
	const GVector3Float result = (in_newPosition - in_oldPosition) / in_timeDelta;
	return result;
}

//constructors
GPhysicsComponentPosition::GPhysicsComponentPosition(
	const float in_massInvert,
	const GVector3Float& in_position,
	const GVector3Float& in_velocity
	)
: mInvertMass(in_massInvert)
, mPosition(in_position)
, mVelocity(in_velocity)
, mForceSum()
{
	return;
}

GPhysicsComponentPosition::GPhysicsComponentPosition(const GPhysicsComponentPosition& in_src)
: mInvertMass(0.0F)
, mPosition()
, mVelocity()
, mForceSum()
{
	(*this) = in_src;
	return;
}

GPhysicsComponentPosition::~GPhysicsComponentPosition()
{
	return;
}

//operators
const GPhysicsComponentPosition& GPhysicsComponentPosition::operator=(const GPhysicsComponentPosition& in_rhs)
{
	if (this != &in_rhs)
	{
		mInvertMass = in_rhs.mInvertMass;
		mPosition = in_rhs.mPosition;
		mVelocity = in_rhs.mVelocity;
		mForceSum = in_rhs.mForceSum;
	}
	return (*this);
}

//public methods
void GPhysicsComponentPosition::ClearForceSum()
{
	mForceSum = GVector3Float::sZero;
	return;
}

void GPhysicsComponentPosition::AddAcceleration(const GVector3Float& in_acceleration)
{
	if (mInvertMass)
	{
		mForceSum += (in_acceleration / mInvertMass);
	}
	return;
}

void GPhysicsComponentPosition::MinusAcceleration(const GVector3Float& in_acceleration)
{
	if (mInvertMass)
	{
		mForceSum -= (in_acceleration / mInvertMass);
	}
	return;
}


void GPhysicsComponentPosition::AddForce(const GVector3Float& in_force)
{
	mForceSum += in_force;
	return;
}

void GPhysicsComponentPosition::Intergrate(const float in_timeDelta)
{
	mVelocity += (mForceSum * (in_timeDelta * mInvertMass));
	mPosition += mVelocity * in_timeDelta;
	return;
}

void GPhysicsComponentPosition::Intergrate(const float in_timeDelta, const float in_velocityDampen)
{
	mVelocity = (mVelocity + (mForceSum * (in_timeDelta * mInvertMass))) * in_velocityDampen;
	mPosition += mVelocity * in_timeDelta;
	return;
}

//set new position and new velocity .ie after collision
void GPhysicsComponentPosition::ManualSetPosition(
	const GVector3Float& in_newPosition, 
	const GVector3Float& in_newVelocity
	)
{
	mPosition = in_newPosition;
	mVelocity = in_newVelocity;
	return;
}
