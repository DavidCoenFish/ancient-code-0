//
//  GSceneNodeComponentNodeAttach.cpp
//
//  Created by David Coen on 2011 01 04
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentNodeAttach.h"

#include "GSceneNode.h"
#include "GSceneType.h"
#include "GCommon.h"
#include "GSceneNodeComponentAnimation.h"
#include "GSceneNodeComponentNodeAttachLoad.h"
#include "GScenePostLoadCallback.h"
#include "GSkeletonInstance.h"
#include "GStringUtility.h"
#include "gassert.h"

//static public methods
/*static*/ GSceneNodeComponentNodeAttach::TPointerSceneNodeComponentBase GSceneNodeComponentNodeAttach::Factory(
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
	const GSceneNodeComponentNodeAttachLoad& componentLoad = *((GSceneNodeComponentNodeAttachLoad*)in_componentData);

	TPointerSceneNode childSceneNode = GSceneNode::Factory(
		inout_scene, 
		inout_arrayPostLoadCallback,
		componentLoad.GetSceneNodeLoad()
		);
	inout_owner->AddChildNode(childSceneNode);

	pointer.reset(new GSceneNodeComponentNodeAttach(
		childSceneNode,
		GStringUtility::SafeString(componentLoad.GetBoneName())//,
		//componentLoad.GetTransformParentRelative()
		));

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		PostLoadCallbackOne	
		));

	return pointer;
}

/*static*/ void GSceneNodeComponentNodeAttach::PostLoadCallbackOne(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	//first round callbacks skeleton may not have been created
	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		inout_component,
		PostLoadCallbackTwo	
		));
	return;
}

/*static*/ void GSceneNodeComponentNodeAttach::PostLoadCallbackTwo(
	GScene& inout_scene,
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentNodeAttach pointer = boost::dynamic_pointer_cast<GSceneNodeComponentNodeAttach>(inout_component);
	if (pointer)
	{
		pointer->AttachToNode(inout_owner);
	}
	return;
}
/*static*/ const unsigned int GSceneNodeComponentNodeAttach::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TNodeAttach;
}

//constructor
GSceneNodeComponentNodeAttach::GSceneNodeComponentNodeAttach(
	TPointerSceneNode& in_childNode,
	const std::string& in_boneName//,
	//const GMatrix16Float& in_boneRelativeMatrix
	)
	: GSceneNodeComponentBase(GSceneNodeComponentNodeAttach::GetComponentFlag())
	, mWeakChildNode(in_childNode)
	, mWeakSkeletonInstance()
	, mBoneName(in_boneName)
	//, mBoneOffsetMatrix(in_boneRelativeMatrix)
	, mBoneIndex(GCOMMON_INVALID_INDEX)
{
	return;
}

/*virtual*/ GSceneNodeComponentNodeAttach::~GSceneNodeComponentNodeAttach()
{
	return;
}

//public accessors
void GSceneNodeComponentNodeAttach::Update(TPointerSceneNode&)
{
	TPointerSkeletonInstance pointerSkeletonInstance = mWeakSkeletonInstance.lock();
	TPointerSceneNode pointerSceneNode = mWeakChildNode.lock();
	if (pointerSkeletonInstance && pointerSceneNode)
	{
		pointerSceneNode->SetParentRelativeTransform(
			//mBoneOffsetMatrix * pointerSkeletonInstance->GetBoneObjectMatrix(mBoneIndex)
			pointerSkeletonInstance->GetBoneObjectMatrix(mBoneIndex)
			);
	}
	return;
}

//private methods
void GSceneNodeComponentNodeAttach::AttachToNode(TPointerSceneNode& inout_sceneNode)
{
	GSceneNode& sceneNode = *inout_sceneNode;
	const int componentCount = sceneNode.GetComponentCount();
	for (int componentIndex = 0; componentIndex < componentCount; ++componentIndex)
	{
		GSceneNodeComponentAnimation* const pAnimation = sceneNode.GetComponent<GSceneNodeComponentAnimation>(componentIndex);
		if (!pAnimation)
		{
			continue;
		}

		const int skeletonCount = pAnimation->GetSkeletonCount();
		for (int skeletonIndex = 0; skeletonIndex < skeletonCount; ++skeletonIndex)
		{
			TPointerSkeletonInstance& pointerSkeletonInstance = pAnimation->GetSkeleton(skeletonIndex);
			if (!pointerSkeletonInstance)
			{
				continue;
			}
			mBoneIndex = pointerSkeletonInstance->GetBoneIndex(mBoneName);
			if (GCOMMON_INVALID_INDEX != mBoneIndex)
			{
				mWeakSkeletonInstance = pointerSkeletonInstance;
				Update(inout_sceneNode);
				return;
			}
		}
	}

	return;
}
