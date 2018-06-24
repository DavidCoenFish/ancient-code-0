//
//  GSceneNodeComponentPointAtBone.cpp
//
//  Created by David Coen on 2010/10/11.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentPointAtBone.h"

#include "GMath.h"
#include "GMatrix16Float.h"
#include "GMatrix9Float.h"
#include "GPhysicsComponentPosition.h"
#include "GSkeletonInstance.h"
#include "GSceneNode.h"
#include "GSceneNodeComponentPointAtBoneLoad.h"
#include "GAssert.h"
#include "GSceneType.h"
#include "GScenePostLoadCallback.h"
#include "GSceneNodeComponentAnimation.h"

#ifdef DSC_DEBUG
#include "GScene.h"
typedef std::vector<GMatrix16Float> TArrayMatrix;
#endif

typedef boost::shared_ptr<GSceneNodeComponentPointAtBone> TPointerSceneNodeComponentPointAtBone;
typedef boost::shared_ptr<GSkeletonInstance> TPointerSkeletonInstance;


//static public methods
/*static*/ GSceneNodeComponentPointAtBone::TPointerSceneNodeComponentBase GSceneNodeComponentPointAtBone::Factory(
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
	const GSceneNodeComponentPointAtBoneLoad& componentLoad = *((GSceneNodeComponentPointAtBoneLoad*)in_componentData);

	pointer.reset(new GSceneNodeComponentPointAtBone(
		componentLoad.GetBoneName(),
		componentLoad.GetMaxAngleDegree(),
		componentLoad.GetLocalAt(),
		componentLoad.GetWorldTarget()
#ifdef DSC_DEBUG
		,&inout_scene
#endif
		));

	inout_arrayPostLoadCallback.push_back(GScenePostLoadCallback(
		inout_owner,
		pointer,
		PostLoadCallbackA	
		));

	return pointer;
}

/*static*/ void GSceneNodeComponentPointAtBone::PostLoadCallbackA(
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

/*static*/ void GSceneNodeComponentPointAtBone::PostLoadCallbackB(
	GScene& inout_scene, //access to mesh/ material manager
	TPointerSceneNode& inout_owner,
	TPointerSceneNodeComponentBase& inout_component,
	TArrayScenePostLoadCallback& inout_arrayPostLoadCallback
	)
{
	TPointerSceneNodeComponentPointAtBone pointerComponent = boost::shared_dynamic_cast<GSceneNodeComponentPointAtBone, GSceneNodeComponentBase>(inout_component);
	GASSERT(NULL != pointerComponent, "invalid state"); 
	GSceneNode& sceneNode = *inout_owner;

	const int componentCount = sceneNode.GetComponentCount();

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
			const int boneIndex = skeletonInstance->GetBoneIndex(pointerComponent->GetName());
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

		pointerComponent->SetBoneIndex(foundBoneIndex);

		if (pAnimation->AddBoneCallback(
			foundBoneIndex,
			foundSkeletonInstance,
			GSceneNodeComponentPointAtBone::BoneUpdateCallback,
			pointerComponent.get()
			))
		{
			break;
		}
	}

	return;
}

/*static*/ const unsigned int GSceneNodeComponentPointAtBone::GetComponentFlag()
{
	return GSceneType::TComponentFlag::TPointAtBone;
}

/*static*/ void GSceneNodeComponentPointAtBone::BoneUpdateCallback(
	GVector3Float& inout_at,
	GVector3Float& inout_up,
	GVector3Float& inout_pos,
	float& inout_scale,
	GSceneNode& in_parentSceneNode,
	const float in_timeDelta,
	void* in_callbackData,
	GSkeletonInstance& inout_skeletonInstance
	)
{
	GSceneNodeComponentPointAtBone* const pThis = (GSceneNodeComponentPointAtBone*)in_callbackData;
	if (!pThis)
	{
		return;
	}

	const int parentIndex = inout_skeletonInstance.GetBoneParentIndex(pThis->m_boneIndex);
	const GMatrix16Float parentBoneWorldMatrix = inout_skeletonInstance.GetBoneObjectMatrix(parentIndex) * in_parentSceneNode.GetWorldTransform();
	const GMatrix16Float invertParentBoneWorldMatrix(parentBoneWorldMatrix.ReturnInverse());

	const GMatrix16Float initialBoneMatrix = GMatrix16FloatConstructAtUp(
		inout_at,
		inout_up,
		GVector3Float::sUnitZ,
		GVector3Float::sUnitY,
		GVector3Float::sZero //  inout_pos
		);

	const GVector3Float parentSpaceTarget = pThis->m_worldTarget * invertParentBoneWorldMatrix;
	const GVector3Float parentSpaceLocalAt = pThis->m_localAt * initialBoneMatrix;

	GR32 radianAngle;
	GVector3Float unitAxis;

	const GBOOL valid = MakeAxisAngle(
		radianAngle,
		unitAxis,
		parentSpaceLocalAt,
		parentSpaceTarget
		);

	if (valid)
	{
		radianAngle = GMath::Clamp(radianAngle, -pThis->m_maxAngleRadian, pThis->m_maxAngleRadian);
		const GMatrix9Float rotationMatrix = GMatrix9FloatConstructAxisAngle(unitAxis, radianAngle);
	
		inout_at = inout_at * rotationMatrix;
		inout_up = inout_up * rotationMatrix;

		pThis->m_currentAngleRadian = radianAngle;
	}
	
//	//debud, draw debug matrix at worldspace of local at
//#ifdef DSC_DEBUG
//	if (pThis->mScene)
//	{
//		GMatrix16Float debugMatrix(GMatrix16Float::sIdentity);
//
//		debugMatrix.SetPosition(pThis->m_worldTarget);
//		pThis->mScene->GetArrayDebugMatrix().push_back(debugMatrix);
//		debugMatrix.SetPosition(pThis->m_worldTarget * 0.5F);
//		pThis->mScene->GetArrayDebugMatrix().push_back(debugMatrix);
//		debugMatrix.SetPosition(pThis->m_worldTarget * 0.6F);
//		pThis->mScene->GetArrayDebugMatrix().push_back(debugMatrix);
//
//	}
//#endif
	return;
}

//constructor
GSceneNodeComponentPointAtBone::GSceneNodeComponentPointAtBone(
	const std::string& in_boneName,
	const float in_maxAngleDegree,
	const GVector3Float& in_localAt,
	const GVector3Float& in_worldTarget
#ifdef DSC_DEBUG
	, GScene* const in_scene
#endif
	)
	: GSceneNodeComponentBase(GetComponentFlag())
	, m_boneName(in_boneName)
	, m_maxAngleRadian((in_maxAngleDegree / 360.0F) * GMath::GetTwoPI())
	, m_localAt(in_localAt)
	, m_boneIndex(GCOMMON_INVALID_INDEX)
	, m_worldTarget(in_worldTarget)
	, m_currentAngleRadian(0.0F)
#ifdef DSC_DEBUG
	, mScene(in_scene)
#endif

{
	return;
}

GSceneNodeComponentPointAtBone::~GSceneNodeComponentPointAtBone()
{
	return;
}
