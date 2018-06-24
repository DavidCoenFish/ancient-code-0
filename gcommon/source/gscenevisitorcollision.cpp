//
//  GSceneVisitorCollision.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneVisitorCollision.h"

#include "GScene.h"
#include "GSceneNode.h"
#include "GCamera.h"
#include "GSceneNodeComponentCollision.h"

typedef boost::shared_ptr<GSceneNodeComponentCollision> TPointerSceneNodeComponentCollision;
typedef boost::weak_ptr<GSceneNodeComponentCollision> TWeakSceneNodeComponentCollision;

//public static methods
/*static*/ void GSceneVisitorCollisionCone::Run(
	GScene& inout_scene,
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	const bool in_wantHitSurfaceNormalObjectSpace,
	const bool in_wantHitSurfaceNormalWorldSpace,
	const GVector3Float& in_worldSpaceRayOrigin,
	const GVector3Float& in_worldSpaceRayDirection,
	const float in_coneLength,
	const float in_radiusOne,
	const float in_radiusTwo	
	)
{
	GSceneVisitorCollisionCone visitor(
		inout_collisionResult,
		in_wantHitSurfaceNormalObjectSpace,
		in_wantHitSurfaceNormalWorldSpace,
		in_worldSpaceRayOrigin,
		in_worldSpaceRayDirection,
		in_coneLength,
		in_radiusOne,
		in_radiusTwo
		);
	inout_scene.QueryAll(
		visitor, 
		GSceneNodeComponentCollision::GetComponentFlag()
		);

	return;
}


/*static*/ void GSceneVisitorCollisionCone::Run(
	TPointerSceneNode& inout_sceneNode,
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	const bool in_wantHitSurfaceNormalObjectSpace,
	const bool in_wantHitSurfaceNormalWorldSpace,
	const GVector3Float& in_worldSpaceRayOrigin,
	const GVector3Float& in_worldSpaceRayDirection,
	const float in_coneLength,
	const float in_radiusOne,
	const float in_radiusTwo
	)
{
	GSceneVisitorCollisionCone visitor(
		inout_collisionResult,
		in_wantHitSurfaceNormalObjectSpace,
		in_wantHitSurfaceNormalWorldSpace,
		in_worldSpaceRayOrigin,
		in_worldSpaceRayDirection,
		in_coneLength,
		in_radiusOne,
		in_radiusTwo
		);
	GSceneNode::QueryAll(
		inout_sceneNode,
		visitor,
		GSceneNodeComponentCollision::GetComponentFlag()
		);

	return;
}

//constructor
GSceneVisitorCollisionCone::GSceneVisitorCollisionCone(
	TArrayPointerSceneNodeComponentCollisionResult& inout_collisionResult,
	const bool in_wantHitSurfaceNormalObjectSpace,
	const bool in_wantHitSurfaceNormalWorldSpace,
	const GVector3Float& in_worldSpaceRayOrigin,
	const GVector3Float& in_worldSpaceRayDirection,
	const float in_coneLength,
	const float in_radiusOne,
	const float in_radiusTwo
	)
	: mCollisionResult(inout_collisionResult)
	, mWantHitSurfaceNormalObjectSpace(in_wantHitSurfaceNormalObjectSpace)
	, mWantHitSurfaceNormalWorldSpace(in_wantHitSurfaceNormalWorldSpace)
	, mWorldSpaceRayOrigin(in_worldSpaceRayOrigin)
	, mWorldSpaceRayDirection(in_worldSpaceRayDirection)
	, mConeLength(in_coneLength)
	, mRadiusOne(in_radiusOne)
	, mRadiusTwo(in_radiusTwo)
{
	return;
}

GSceneVisitorCollisionCone::~GSceneVisitorCollisionCone()
{
	return;
}

//public methods
void GSceneVisitorCollisionCone::QuerySubmitNode(
	TPointerSceneNode& inout_node
	)
{
	GSceneNode& sceneNode = *inout_node;
	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		TPointerSceneNodeComponentCollision pComponentCollision = sceneNode.GetComponentPointer<GSceneNodeComponentCollision>(index);
		if (pComponentCollision)
		{
			GSceneNodeComponentCollision& componentCollision = *pComponentCollision;
			if (componentCollision.GetActive())
			{
				TWeakSceneNodeComponentCollision weak = pComponentCollision;
				componentCollision.CollideCone(
					mCollisionResult,
					weak,
					mWantHitSurfaceNormalObjectSpace,
					mWantHitSurfaceNormalWorldSpace,
					mWorldSpaceRayOrigin,
					mWorldSpaceRayDirection,
					mConeLength,
					mRadiusOne,
					mRadiusTwo
					);
			}
		}
	}

	return;
}

void GSceneVisitorCollisionCone::QueryFinish()
{
	return;
}

//public static methods
/*static*/ void GSceneVisitorCollisionCollect::Run(
	TPointerSceneNode& inout_sceneNode,
	const std::string& in_data,
	TArrayWeakSceneNodeComponentCollision& out_arrayCollision
	)
{
	GSceneVisitorCollisionCollect visitor(
		in_data,
		out_arrayCollision
		);
	GSceneNode::QueryAll(
		inout_sceneNode,
		visitor,
		GSceneNodeComponentCollision::GetComponentFlag()
		);

	return;
}

//constructor
GSceneVisitorCollisionCollect::GSceneVisitorCollisionCollect(
	const std::string& in_data,
	TArrayWeakSceneNodeComponentCollision& out_arrayCollision
	)
	: mData(in_data)
	, mArrayCollision(out_arrayCollision)
{
	return;
}

GSceneVisitorCollisionCollect::~GSceneVisitorCollisionCollect()
{
	return;
}

//public methods
void GSceneVisitorCollisionCollect::QuerySubmitNode(
	TPointerSceneNode& inout_node
	)
{
	GSceneNode& sceneNode = *inout_node;
	const int componentCount = sceneNode.GetComponentCount();
	for (int index = 0; index < componentCount; ++index)
	{
		TPointerSceneNodeComponentCollision pComponentCollision = sceneNode.GetComponentPointer<GSceneNodeComponentCollision>(index);
		if ((pComponentCollision) &&
			(mData == pComponentCollision->GetData()))
		{
			mArrayCollision.push_back(pComponentCollision);
		}
	}
	return;
}

void GSceneVisitorCollisionCollect::QueryFinish()
{
	return;
}
