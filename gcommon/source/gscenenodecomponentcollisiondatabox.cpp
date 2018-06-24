//
//  GSceneNodeComponentCollisionDataBox.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionDataBox.h"

#include "GSceneNodeLoad.h"
#include "GAssert.h"

//constructor
GSceneNodeComponentCollisionDataBox::GSceneNodeComponentCollisionDataBox(
	const GVector3Float& in_low,
	const GVector3Float& in_high
	)
	: mLow(in_low)
	, mHigh(in_high)
{
	return;
}

GSceneNodeComponentCollisionDataBox::~GSceneNodeComponentCollisionDataBox()
{
	return;
}

//implement GSceneNodeComponentCollisionDataBase
/*virtual*/ void GSceneNodeComponentCollisionDataBox::OnCollideRay(
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	TWeakSceneNodeComponentCollision& in_weakOwner,
	const bool in_wantHitSurfaceNormal,
	const GVector3Float& in_objectSpaceRayOrigin,
	const GVector3Float& in_objectSpaceRayDirection
	)
{
	GASSERT(false, "TODO");
	return;
}

/*virtual*/ void GSceneNodeComponentCollisionDataBox::OnCollideCone(
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	TWeakSceneNodeComponentCollision& in_weakOwner,
	const bool in_wantHitSurfaceNormal,
	const GVector3Float& in_objectSpaceRayOrigin,
	const GVector3Float& in_objectSpaceRayDirection,
	const float in_coneLength,
	const float in_radiusOne,
	const float in_radiusTwo
	)
{
	GASSERT(false, "TODO");
	return;
}

