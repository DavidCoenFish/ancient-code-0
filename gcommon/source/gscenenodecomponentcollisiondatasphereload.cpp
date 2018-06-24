//
//  GSceneNodeComponentCollisionDataSphereLoad.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionDataSphereLoad.h"

//constructor
GSceneNodeComponentCollisionDataSphereLoad::GSceneNodeComponentCollisionDataSphereLoad(
	const float in_radius,
	const GVector3Float* const in_origin
	)
	: mRadius(in_radius)
	, mOrigin(in_origin)
{
	return;
}

GSceneNodeComponentCollisionDataSphereLoad::~GSceneNodeComponentCollisionDataSphereLoad()
{
	return;
}
