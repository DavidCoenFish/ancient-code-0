//
//  GPhysicsComponentSpring.cpp
//
//  Created by David Coen on 2010 10 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GPhysicsComponentSpring.h"
#include "GVector3Float.h"

//constructors
GPhysicsComponentSpring::GPhysicsComponentSpring(
	const float in_springConstant,
	const float in_springInnerFriction
	)
: mSpringConstant(in_springConstant)
, mSpringInnerFriction(in_springInnerFriction)
{
	return;
}

GPhysicsComponentSpring::GPhysicsComponentSpring(const GPhysicsComponentSpring& in_src)
: mSpringConstant(0.0F)
, mSpringInnerFriction(0.0F)
{
	(*this) = in_src;
	return;
}

GPhysicsComponentSpring::~GPhysicsComponentSpring()
{
	return;
}

//operators
const GPhysicsComponentSpring& GPhysicsComponentSpring::operator=(const GPhysicsComponentSpring& in_rhs)
{
	if (this != &in_rhs)
	{
		mSpringConstant = in_rhs.mSpringConstant;
		mSpringInnerFriction = in_rhs.mSpringInnerFriction;
	}
	return (*this);
}

//public methods
const float GPhysicsComponentSpring::CalculateSpringForce(
	const float in_target, //the distance we want to be from the spring origin
	const float in_currentValue
	)const 
{
	const float force = -((in_currentValue - in_target) * mSpringConstant);
	return force;
}

const float GPhysicsComponentSpring::CalculateSpringFriction(
	const float in_velocityLhs,
	const float in_velocityRhs
	)const
{
	const float frictionForce = -(in_velocityLhs - in_velocityRhs) * mSpringInnerFriction;
	return frictionForce;
}

const GVector3Float GPhysicsComponentSpring::CalculateSpringForce(
	const GVector3Float& in_origin,
	const float in_targetDistance, //the distance we want to be from the spring origin
	const GVector3Float& in_currentValue
	)const
{
	float currentDistance(0.0F);
	const GVector3Float offsetNormal = NormaliseAprox(in_currentValue - in_origin, currentDistance);
	const float force = -((currentDistance - in_targetDistance) * mSpringConstant);
	const GVector3Float forceVector = offsetNormal * force;
	return forceVector;
}

const GVector3Float GPhysicsComponentSpring::CalculateSpringFriction(
	const GVector3Float& in_velocityLhs,
	const GVector3Float& in_velocityRhs
	)const
{
	const GVector3Float frictionForce = -(in_velocityLhs - in_velocityRhs) * mSpringInnerFriction;
	return frictionForce;
}
