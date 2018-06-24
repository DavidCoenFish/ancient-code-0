//
//  GSceneNodeComponentSpringBone.cpp
//
//  Created by David Coen on 2010/10/11.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentSpringBone.h"

#include "GSceneType.h"
#include "GSceneNodeComponentSpringBoneData.h"
#include "GSceneNodeComponentSpringBoneLoad.h"
#include "GSceneNodeComponentSpringBoneDataLoad.h"
#include "GSceneNodeComponentAnimation.h"
#include "GSceneNodeComponentVisual.h"
#include "GScenePostLoadCallback.h"
#include "GSceneNode.h"
#include "GAssert.h"
#include "GSkeletonInstance.h"


typedef boost::shared_ptr<GSceneNodeComponentBase> TPointerSceneNodeComponentBase;
typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;
typedef boost::weak_ptr<GSkeletonInstance> TWeakSkeletonInstance;

//static public methods
/*static*/ GSceneNodeComponentSpringBone::TPointerSceneNodeComponentBase GSceneNodeComponentSpringBone::Factory(
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
	const GSceneNodeComponentSpringBoneLoad& componentLoad = *((GSceneNodeComponentSpringBoneLoad*)in_componentData);

	TArrayPointerSceneNodeComponentSpringBoneData arraySpringBoneData;
	for (int index = 0; index < componentLoad.GetArraySpringBoneDataCount(); ++index)
	{
		TPointerSceneNodeComponentSpringBoneData pointerData = GSceneNodeComponentSpringBoneData::Factory(
			componentLoad.GetArraySpringBoneData()[index]
#ifdef DSC_DEBUG
			, &inout_scene
#endif
			);
		if (!pointerData)
		{
			continue;
		}
		arraySpringBoneData.push_back(pointerData);
	}

	pointer.reset(new GSceneNodeComponentSpringBone(arraySpringBoneData));

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		PostLoadCallbackA	
		));

	return pointer;
}
/*static*/ void GSceneNodeComponentSpringBone::PostLoadCallbackA(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		inout_component,
		PostLoadCallbackB	
		));
	return;
}

/*static*/ void GSceneNodeComponentSpringBone::PostLoadCallbackB(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentSpringBone pointerComponent = boost::shared_dynamic_cast<GSceneNodeComponentSpringBone, GSceneNodeComponentBase>(inout_component);
	GASSERT(NULL != pointerComponent, "invalid state"); 
	GSceneNode& sceneNode = *inout_owner;

	const int componentCount = sceneNode.GetComponentCount();

	//we want to find the bone the each spring will effect
	for (TArrayPointerSceneNodeComponentSpringBoneData::iterator iterator = pointerComponent->mArrayBoneData.begin(); iterator != pointerComponent->mArrayBoneData.end(); ++iterator)
	{
		TPointerSceneNodeComponentSpringBoneData& springBone = (*iterator);

		//find the bone index and renderMeshInstance target
		int foundBoneIndex = GCOMMON_INVALID_INDEX;
		TPointerSkeletonInstance foundSkeletonInstance;
		for (int index = 0; index < componentCount; ++index)
		{
			GSceneNodeComponentAnimation* const pAnimation = sceneNode.GetComponent<GSceneNodeComponentAnimation>(index);
			if (!pAnimation)
			{
				continue;
			}

			for (int subIndex = 0; subIndex < pAnimation->GetSkeletonCount(); ++subIndex)
			{
				TPointerSkeletonInstance& skeletonInstance = pAnimation->GetSkeleton(subIndex);
				const int boneIndex = skeletonInstance->GetBoneIndex(springBone->GetName());
				if (GCOMMON_INVALID_INDEX == boneIndex)
				{
					continue;
				}

				foundBoneIndex = boneIndex;
				foundSkeletonInstance = skeletonInstance;

				break;
			}

			if (GCOMMON_INVALID_INDEX == foundBoneIndex)
			{
				continue;
			}

			springBone->SetBoneIndex(foundBoneIndex);

			if (pAnimation->AddBoneCallback(
				foundBoneIndex,
				foundSkeletonInstance,
				GSceneNodeComponentSpringBoneData::BoneUpdateCallback,
				springBone.get()
				))
			{
				break;
			}
		}
	}

	return;
}

/*static*/ const unsigned int GSceneNodeComponentSpringBone::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TSpringBone;
}

//constructor
GSceneNodeComponentSpringBone::GSceneNodeComponentSpringBone(TArrayPointerSceneNodeComponentSpringBoneData& in_springBoneData)
: GSceneNodeComponentBase(GetComponentFlag())
, mArrayBoneData(in_springBoneData)
{
	return;
}

/*virtual*/ GSceneNodeComponentSpringBone::~GSceneNodeComponentSpringBone()
{
	return;
}
