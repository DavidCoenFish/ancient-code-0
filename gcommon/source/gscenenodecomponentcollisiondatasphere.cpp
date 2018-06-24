//
//  GSceneNodeComponentCollisionDataSphere.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionDataSphere.h"

#include "GSceneNodeLoad.h"
#include "GAssert.h"
#include "GSceneNodeComponentCollisionResult.h"
#include "GMath.h"

//constructor
GSceneNodeComponentCollisionDataSphere::GSceneNodeComponentCollisionDataSphere(
	const float in_radius,
	const GVector3Float& in_origin
	)
	: mRadius(in_radius)
	, mOrigin(in_origin)
{
	return;
}

GSceneNodeComponentCollisionDataSphere::~GSceneNodeComponentCollisionDataSphere()
{
	return;
}

//implement GSceneNodeComponentCollisionDataBase
/*virtual*/ void GSceneNodeComponentCollisionDataSphere::OnCollideRay(
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	TWeakSceneNodeComponentCollision& in_weakOwner,
	const bool in_wantHitSurfaceNormal,
	const GVector3Float& in_objectSpaceRayOrigin,
	const GVector3Float& in_objectSpaceRayDirection
	)
{
	float rayRatio(0.0F);

	RayPointClosestPointOfApproach(
		rayRatio,
		in_objectSpaceRayOrigin,
		in_objectSpaceRayDirection,
		mOrigin
		);

	const GVector3Float rayClosestPoint = ProjectRay(
		in_objectSpaceRayOrigin, 
		in_objectSpaceRayDirection, 
		rayRatio
		);

	const float distanceSquared = LengthSquared(rayClosestPoint - mOrigin);
	if ((mRadius * mRadius) < distanceSquared)
	{
		return;
	}

	GVector3Float surfaceNormal;
	if (in_wantHitSurfaceNormal)
	{
		surfaceNormal = rayClosestPoint - mOrigin;
	}

	TPointerSceneNodeComponentCollisionResult pointerResult;
	pointerResult.reset(new GSceneNodeComponentCollisionResult(
		in_weakOwner,
		rayRatio,
		surfaceNormal,
		false
		));

	inout_collisionResult.push_back(pointerResult);

	return;
}

/*virtual*/ void GSceneNodeComponentCollisionDataSphere::OnCollideCone(
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
	float rayRatio(0.0F);

	RayPointClosestPointOfApproach(
		rayRatio,
		in_objectSpaceRayOrigin,
		in_objectSpaceRayDirection,
		mOrigin
		);
	const float normalisedRayRatio = rayRatio / in_coneLength;
	const float coneRadius = GMath::Lerp(GMath::Clamp(normalisedRayRatio, 0.0F, 1.0F), in_radiusOne, in_radiusTwo);

	const GVector3Float rayClosestPoint = ProjectRay(
		in_objectSpaceRayOrigin, 
		in_objectSpaceRayDirection, 
		GMath::Clamp(rayRatio, 0.0F, in_coneLength)
		);

	const float distanceSquared = LengthSquared(mOrigin - rayClosestPoint);

	const float temp = mRadius + coneRadius;
	const float hitDistance = temp * temp;

	const bool collision = (distanceSquared <= hitDistance);
	if (!collision)
	{
		return;
	}

	GVector3Float surfaceNormal;
	if (in_wantHitSurfaceNormal)
	{
		surfaceNormal = rayClosestPoint - mOrigin;
	}

	TPointerSceneNodeComponentCollisionResult pointerResult;
	pointerResult.reset(new GSceneNodeComponentCollisionResult(
		in_weakOwner,
		rayRatio - mRadius,
		surfaceNormal,
		false
		));

	inout_collisionResult.push_back(pointerResult);

	return;
}