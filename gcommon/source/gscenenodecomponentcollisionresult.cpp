//
//  GSceneNodeComponentCollisionResult.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionResult.h"
#include "GMatrix9Float.h"

//constructor
GSceneNodeComponentCollisionResult::GSceneNodeComponentCollisionResult(
	TWeakSceneNodeComponentCollision& in_weakOwner,
	const float in_hitRatio,
	const GVector3Float& in_hitSurfaceNormal,
	const bool in_hitSurfaceNormalalised
	)
	: mHitRatio(in_hitRatio)
	, mHitSurfaceNormal(in_hitSurfaceNormal)
	, mHitSurfaceNormalalised(in_hitSurfaceNormalalised)
	, mWeakComponentCollision(in_weakOwner)
{
	return;
}

GSceneNodeComponentCollisionResult::~GSceneNodeComponentCollisionResult()
{
	return;
}

//public methods
void GSceneNodeComponentCollisionResult::PutNormalIntoWorldSpace(const GMatrix9Float& in_worldRotation)
{
	mHitSurfaceNormal = mHitSurfaceNormal * in_worldRotation;
	return;
}

//public accessors
const GVector3Float& GSceneNodeComponentCollisionResult::GetHitSuraceNormal()
{
	if (!mHitSurfaceNormalalised)
	{
		mHitSurfaceNormalalised = true;
		mHitSurfaceNormal = Normalise(mHitSurfaceNormal);
	}
	return mHitSurfaceNormal;
}
