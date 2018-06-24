//
//  GPhysicsComponentLimit.cpp
//
//  Created by David Coen on 2010 10 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GPhysicsComponentLimit.h"

#include "GVector3Float.h"

//constructors
GPhysicsComponentLimit::GPhysicsComponentLimit(
	const float in_maxDistance
	)
: mMaxDistance(in_maxDistance)
{
	return;
}

GPhysicsComponentLimit::GPhysicsComponentLimit(const GPhysicsComponentLimit& in_src)
: mMaxDistance(0.0F)
{
	(*this) = in_src;
	return;
}

GPhysicsComponentLimit::~GPhysicsComponentLimit()
{
	return;
}

//operators
const GPhysicsComponentLimit& GPhysicsComponentLimit::operator=(const GPhysicsComponentLimit& in_rhs)
{
	if (this != &in_rhs)
	{
		mMaxDistance = in_rhs.mMaxDistance;
	}
	return (*this);
}

//public methods
//return true if target value was changed by limit
const bool GPhysicsComponentLimit::CalculateLimit(
	float& inout_currentValue,
	const float in_targetValue
	)const
{
	const float delta = inout_currentValue - in_targetValue;
	if (delta < -mMaxDistance)
	{
		inout_currentValue = in_targetValue - mMaxDistance;
		return true;
	}

	if (mMaxDistance < delta)
	{
		inout_currentValue = in_targetValue + mMaxDistance;
		return true;
	}

	return false;
}

//return true if target value was changed by limit
const bool GPhysicsComponentLimit::CalculateLimit(
	GVector3Float& inout_currentValue,
	const GVector3Float& in_targetValue
	)const
{
	const float maxDistanceSquared = mMaxDistance * mMaxDistance;
	const GVector3Float offset = inout_currentValue - in_targetValue;
	const float lengthSquared = LengthSquared(offset);
	if (maxDistanceSquared < lengthSquared)
	{
		const GVector3Float normal = NormaliseAprox(offset);
		inout_currentValue = (in_targetValue + (normal * mMaxDistance));
		return true;
	}

	return false;
}

const bool GPhysicsComponentLimit::CalculateLimit(
	GVector3Float& out_newPosition,
	GVector3Float& out_newVelocity,
	const GVector3Float& in_position,
	const GVector3Float& in_velocity,
	const GVector3Float& in_origin,
	const float in_timeDelta
	)const
{
	out_newPosition = in_position;
	if (CalculateLimit(out_newPosition, in_origin))
	{
		//calculate new velocity
		if (in_timeDelta)
		{
			const GVector3Float prevPosition = (in_position - (in_velocity * in_timeDelta));
			out_newVelocity = (out_newPosition - prevPosition) / in_timeDelta;
		}
		return true;
	}
	return false;
}
