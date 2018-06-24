//
//  GSceneNodeComponentCollisionDataCapsualLoad.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionDataCapsualLoad.h"

//constructor
GSceneNodeComponentCollisionDataCapsualLoad::GSceneNodeComponentCollisionDataCapsualLoad(
	const GVector3Float* const in_origin,
	const GVector3Float* const in_direction,
	const float in_radius,
	const float in_length
	)
	: mOrigin(in_origin)
	, mDirection(in_direction)
	, mRadius(in_radius)
	, mLength(in_length)
{
	return;
}

GSceneNodeComponentCollisionDataCapsualLoad::~GSceneNodeComponentCollisionDataCapsualLoad()
{
	return;
}
