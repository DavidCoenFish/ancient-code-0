//
//  GSceneNodeComponentCollision.cpp
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollision.h"

#include "GSceneNode.h"
#include "GSceneNodeComponentCollisionLoad.h"
#include "GSceneNodeComponentCollisionDataLoad.h"
#include "GSceneNodeComponentCollisionDataBase.h"
#include "GSceneNodeComponentCollisionResult.h"
#include "GSceneType.h"
#include "GMatrix9Float.h"
#include "GStringUtility.h"

//static public methods
/*static*/ GSceneNodeComponentCollision::TPointerSceneNodeComponentBase GSceneNodeComponentCollision::Factory(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner, //pain, some components may want access to owner. means we need to create components after node, so node needs ablity to add component
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback,
	const void* const in_componentData
	)
{
	TPointerSceneNodeComponentBase pointer;
	if (!in_componentData)
	{
		return pointer;
	}
	const GSceneNodeComponentCollisionLoad& componentLoad = *((GSceneNodeComponentCollisionLoad*)in_componentData);

	TArrayPointerSceneNodeComponentCollisionDataBase arrayCollisionDataBase;
	for (int index = 0; index < componentLoad.GetCollisionDataCount(); ++index)
	{
		const GSceneNodeComponentCollisionDataLoad& dataLoad = componentLoad.GetArrayCollisionData()[index];
		TPointerSceneNodeComponentCollisionDataBase pointerData = dataLoad.Factory();
		if (pointerData)
		{
			arrayCollisionDataBase.push_back(pointerData);
		}
	}

	pointer.reset(new GSceneNodeComponentCollision(
		*inout_owner,
		componentLoad.GetMaterialId(),
		GStringUtility::SafeString(componentLoad.GetData()),
		arrayCollisionDataBase,
		componentLoad.GetFlagActive()
		));

	return pointer;
}

/*static*/ const unsigned int GSceneNodeComponentCollision::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TCollision;
}

//constructor
GSceneNodeComponentCollision::GSceneNodeComponentCollision(
	GSceneNode& in_parentSceneNode,
	const int in_materialId,
	const std::string& in_data,
	TArrayPointerSceneNodeComponentCollisionDataBase& in_arrayData, //WARN: transfer ownership
	const bool in_active
	)
	: GSceneNodeComponentBase(GetComponentFlag())
	, mParentSceneNode(in_parentSceneNode)
	, mMaterialId(in_materialId)
	, mData(in_data)
	, mArrayData(in_arrayData)
	, mActive(in_active)
{
	return;
}

GSceneNodeComponentCollision::~GSceneNodeComponentCollision()
{
	return;
}

//public methods
void GSceneNodeComponentCollision::CollideRay(
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	TWeakSceneNodeComponentCollision& in_weakOwner,
	const bool in_wantHitSurfaceNormalObjectSpace,
	const bool in_wantHitSurfaceNormalWorldSpace,
	const GVector3Float& in_worldSpaceRayOrigin,
	const GVector3Float& in_worldSpaceRayDirection
	)
{
	const GMatrix16Float& worldTransform = mParentSceneNode.GetWorldTransform();
	const GMatrix9Float rotation = worldTransform.GetRotation();
	GMatrix9Float invertRotation(rotation);
	invertRotation.TransposeSelf();
	const GVector3Float objectSpaceRayOrigin = (in_worldSpaceRayOrigin - worldTransform.GetPosition()) * invertRotation;
	const GVector3Float objectSpaceRayDirection = in_worldSpaceRayDirection * invertRotation;

	for (TArrayPointerSceneNodeComponentCollisionDataBase::iterator iterator = mArrayData.begin(); iterator != mArrayData.end(); ++iterator)
	{
		(*iterator)->CollideRay(
			inout_collisionResult,
			in_weakOwner,
			(in_wantHitSurfaceNormalObjectSpace || in_wantHitSurfaceNormalWorldSpace),
			objectSpaceRayOrigin,
			objectSpaceRayDirection
			);
	}

	if (in_wantHitSurfaceNormalWorldSpace)
	{
		for (TArrayPointerSceneNodeComponentCollisionResult::iterator iterator = inout_collisionResult.begin(); iterator != inout_collisionResult.end(); ++iterator)
		{
			(*iterator)->PutNormalIntoWorldSpace(rotation);
		}
	}

	return;
}

void GSceneNodeComponentCollision::CollideCone(
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	TWeakSceneNodeComponentCollision& in_weakOwner,
	const bool in_wantHitSurfaceNormalObjectSpace,
	const bool in_wantHitSurfaceNormalWorldSpace,
	const GVector3Float& in_worldSpaceRayOrigin,
	const GVector3Float& in_worldSpaceRayDirection,
	const float in_coneLength,
	const float in_radiusOne,
	const float in_radiusTwo
	)
{
	const GMatrix16Float& worldTransform = mParentSceneNode.GetWorldTransform();
	const GMatrix9Float rotation = worldTransform.GetRotation();
	GMatrix9Float invertRotation(rotation);
	invertRotation.TransposeSelf();
	const GVector3Float objectSpaceRayOrigin = (in_worldSpaceRayOrigin - worldTransform.GetPosition()) * invertRotation;
	const GVector3Float objectSpaceRayDirection = in_worldSpaceRayDirection * invertRotation;

	for (TArrayPointerSceneNodeComponentCollisionDataBase::iterator iterator = mArrayData.begin(); iterator != mArrayData.end(); ++iterator)
	{
		(*iterator)->CollideCone(
			inout_collisionResult,
			in_weakOwner,
			(in_wantHitSurfaceNormalObjectSpace || in_wantHitSurfaceNormalWorldSpace),
			objectSpaceRayOrigin,
			objectSpaceRayDirection,
			in_coneLength,
			in_radiusOne,
			in_radiusTwo
			);
	}


	if (in_wantHitSurfaceNormalWorldSpace)
	{
		for (TArrayPointerSceneNodeComponentCollisionResult::iterator iterator = inout_collisionResult.begin(); iterator != inout_collisionResult.end(); ++iterator)
		{
			(*iterator)->PutNormalIntoWorldSpace(rotation);
		}
	}

	return;
}
