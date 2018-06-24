//
//  GSceneNodeComponentPhysicsLoad.cpp
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentPhysicsLoad.h"

#include "GSceneNodeLoad.h"
#include "GAssert.h"

//constructor
GSceneNodeComponentPhysicsLoad::GSceneNodeComponentPhysicsLoad(
	const float in_massInvert,
	const GVector3Float* const in_initialVelocity,
	const int in_flag
	)
	: mMassInvert(in_massInvert)
	, mInitialVelocity(in_initialVelocity)
	, mFlag(in_flag)
{
	return;
}

GSceneNodeComponentPhysicsLoad::~GSceneNodeComponentPhysicsLoad()
{
	return;
}
