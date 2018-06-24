//
//  GSceneNodeComponentCollisionDataBase.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionDataBase.h"

//constructor
GSceneNodeComponentCollisionDataBase::GSceneNodeComponentCollisionDataBase()
{
	return;
}

GSceneNodeComponentCollisionDataBase::~GSceneNodeComponentCollisionDataBase()
{
	return;
}


//public methods
void GSceneNodeComponentCollisionDataBase::CollideRay(
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	TWeakSceneNodeComponentCollision& in_weakOwner,
	const bool in_wantHitSurfaceNormal,
	const GVector3Float& in_objectSpaceRayOrigin,
	const GVector3Float& in_objectSpaceRayDirection
	)
{
	OnCollideRay(
		inout_collisionResult,
		in_weakOwner,
		in_wantHitSurfaceNormal,
		in_objectSpaceRayOrigin,
		in_objectSpaceRayDirection
		);
	return;
}

void GSceneNodeComponentCollisionDataBase::CollideCone(
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
	OnCollideCone(
		inout_collisionResult,
		in_weakOwner,
		in_wantHitSurfaceNormal,
		in_objectSpaceRayOrigin,
		in_objectSpaceRayDirection,
		in_coneLength,
		in_radiusOne,
		in_radiusTwo
		);
	return;
}

//private interface
/*virtual*/ void GSceneNodeComponentCollisionDataBase::OnCollideRay(
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	TWeakSceneNodeComponentCollision& in_weakOwner,
	const bool in_wantHitSurfaceNormal,
	const GVector3Float& in_objectSpaceRayOrigin,
	const GVector3Float& in_objectSpaceRayDirection
	)
{
	return;
}

/*virtual*/ void GSceneNodeComponentCollisionDataBase::OnCollideCone(
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
	return;
}

