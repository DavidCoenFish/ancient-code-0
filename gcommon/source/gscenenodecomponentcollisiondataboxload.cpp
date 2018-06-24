//
//  GSceneNodeComponentCollisionDataBoxLoad.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionDataBoxLoad.h"

#include "GSceneNodeLoad.h"
#include "GAssert.h"

//constructor
GSceneNodeComponentCollisionDataBoxLoad::GSceneNodeComponentCollisionDataBoxLoad(
	const GVector3Float* const in_low,
	const GVector3Float* const in_high
	)
	: mLow(in_low)
	, mHigh(in_high)
{
	return;
}

GSceneNodeComponentCollisionDataBoxLoad::~GSceneNodeComponentCollisionDataBoxLoad()
{
	return;
}
