//
//  GSceneNodeComponentCollisionDataCapsual.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionDataCapsual.h"

#include "GSceneNodeLoad.h"
#include "GSceneNodeComponentCollisionResult.h"
#include "GAssert.h"
#include "GMath.h"
#include "GVector3Float.h"

//constructor
GSceneNodeComponentCollisionDataCapsual::GSceneNodeComponentCollisionDataCapsual(
	const GVector3Float& in_origin,
	const GVector3Float& in_direction,
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

GSceneNodeComponentCollisionDataCapsual::~GSceneNodeComponentCollisionDataCapsual()
{
	return;
}

//implement GSceneNodeComponentCollisionDataBase
/*virtual*/ void GSceneNodeComponentCollisionDataCapsual::OnCollideRay(
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	TWeakSceneNodeComponentCollision& in_weakOwner,
	const bool in_wantHitSurfaceNormal,
	const GVector3Float& in_objectSpaceRayOrigin,
	const GVector3Float& in_objectSpaceRayDirection
	)
{
	float rayRatio(0.0F);
	float capsualRatio(0.0F);

	RayRayClosestPointOfApproach(
		rayRatio,
		capsualRatio,
		in_objectSpaceRayOrigin,
		in_objectSpaceRayDirection, 
		mOrigin,
		mDirection
		);

	//is capsualRatio within range
	if ((capsualRatio < -mRadius) || 
		((mLength + mRadius) < capsualRatio))
	{
		return;
	}

	//check distance between points
	const GVector3Float rayClosestPoint = ProjectRay(in_objectSpaceRayOrigin, in_objectSpaceRayDirection, rayRatio);
	const GVector3Float capsualClosestPoint = ProjectRay(
		mOrigin, 
		mDirection,
		//GMath::Clamp(capsualRatio / mLength, 0.0F, 1.0F) * mLength
		GMath::Clamp(capsualRatio, 0.0F, mLength)
		);

	const float distanceSquared = LengthSquared(rayClosestPoint - capsualClosestPoint);
	if ((mRadius * mRadius) < distanceSquared)
	{
		return;
	}

	GVector3Float surfaceNormal;
	if (in_wantHitSurfaceNormal)
	{
		surfaceNormal = rayClosestPoint - capsualClosestPoint;
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

/*virtual*/ void GSceneNodeComponentCollisionDataCapsual::OnCollideCone(
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
	float capsualRatio(0.0F);

	RayRayClosestPointOfApproach(
		rayRatio,
		capsualRatio,
		in_objectSpaceRayOrigin,
		in_objectSpaceRayDirection, 
		mOrigin,
		mDirection 
		);

	const float normalisedRayRatio = rayRatio / in_coneLength;
	const float coneRadius = GMath::Lerp(GMath::Clamp(normalisedRayRatio, 0.0F, 1.0F), in_radiusOne, in_radiusTwo);

	const GVector3Float coneClosestPoint = ProjectRay(
		in_objectSpaceRayOrigin, 
		in_objectSpaceRayDirection, 
		GMath::Clamp(rayRatio, 0.0F, in_coneLength)
		);
	const GVector3Float capsualClosestPoint = ProjectRay(
		mOrigin, 
		mDirection,
		//GMath::Clamp(capsualRatio / mLength, 0.0F, 1.0F) * mLength
		GMath::Clamp(capsualRatio, 0.0F, mLength)
		);

	const float distanceSquared = LengthSquared(capsualClosestPoint - coneClosestPoint);

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
		surfaceNormal = coneClosestPoint - capsualClosestPoint;
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

