//
//  GSceneNodeComponentAnimationAction.cpp
//
//  Created by David Coen on 10/10/05.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentAnimationAction.h"

#include "GSceneNode.h"
#include "GSceneNodeComponentVisual.h"
#include "GMesh.h"
#include "GAnimationManagerInstance.h"
#include "GCommon.h"
#include "GAssert.h"
#include "GSkeletonInstance.h"

//typedef
struct TType
{
	enum TEnum
	{
		TSceneNodeAnimation = 0,
		TBoneAnimation,
		TMorphTarget,

		TCount
	};
};

//public static methods
/*static*/ void GSceneNodeComponentAnimationAction::Factory(
	TArrayPointerSceneNodeComponentAnimationAction& out_arrayAction,
	TPointerAnimationManagerInstance& in_animationManager,
	TPointerSkeletonInstance& in_skeletonInstance
	)
{
	if (!in_animationManager || !in_skeletonInstance)
	{
		return;
	}

	//does the animation manager have any streams targeting a scene node
	const int boneCount = in_skeletonInstance->GetBoneCount();
	for (int boneIndex = 0; boneIndex < boneCount; ++boneIndex)
	{
		const std::string boneName = in_skeletonInstance->GetBoneName(boneIndex);
		int streamIndexPosition = in_animationManager->GetStreamIndex(boneName, GAnimationType::TStreamType::TPosition);
		int streamIndexRotation = in_animationManager->GetStreamIndex(boneName, GAnimationType::TStreamType::TRotation);
		int streamIndexScale = in_animationManager->GetStreamIndex(boneName, GAnimationType::TStreamType::TScale);

		if ((GCOMMON_INVALID_INDEX != streamIndexPosition) ||
			(GCOMMON_INVALID_INDEX != streamIndexRotation) ||
			(GCOMMON_INVALID_INDEX != streamIndexScale))
		{
			TPointerSceneNodeComponentAnimationAction pointer;
			pointer.reset(new GSceneNodeComponentAnimationAction(
				TType::TBoneAnimation,
				streamIndexPosition, 
				streamIndexRotation,
				streamIndexScale,
				in_skeletonInstance,
				boneIndex
				));


			out_arrayAction.push_back(pointer);
		}
	}

	//
	const int morphTargetCount = in_skeletonInstance->GetMorphTargetCount();
	for (int morphTargetIndex = 0; morphTargetIndex < morphTargetCount; ++morphTargetIndex)
	{
		const std::string morphTargetName = in_skeletonInstance->GetMorphTargetName(morphTargetIndex);
		int streamIndex = in_animationManager->GetStreamIndex(morphTargetName, GAnimationType::TStreamType::TMorphWeight);

		if (GCOMMON_INVALID_INDEX != streamIndex)
		{
			TPointerSceneNodeComponentAnimationAction pointer;
			pointer.reset(new GSceneNodeComponentAnimationAction(
				TType::TMorphTarget,
				streamIndex, 
				GCOMMON_INVALID_INDEX,
				GCOMMON_INVALID_INDEX,
				in_skeletonInstance,
				morphTargetIndex
				));


			out_arrayAction.push_back(pointer);
		}
	}

	return;
}

//constructor
GSceneNodeComponentAnimationAction::GSceneNodeComponentAnimationAction(
	const int in_type,
	const int in_streamIndexPosition,
	const int in_streamIndexRotation,
	const int in_streamIndexScale,
	TPointerSkeletonInstance& inout_skeletonInstance,
	const int in_boneIndex
	)
	: mType(in_type)
	, mStreamIndexPosition(in_streamIndexPosition)
	, mStreamIndexRotation(in_streamIndexRotation)
	, mStreamIndexScale(in_streamIndexScale)
	, mWeakSkeletonInstance(inout_skeletonInstance)
	, mBoneIndex(in_boneIndex)
	, mBoneUpdateCallback(NULL)
	, mBoneCallbackData(NULL)
{
	return;
}

GSceneNodeComponentAnimationAction::~GSceneNodeComponentAnimationAction()
{
	return;
}

//public methods
void GSceneNodeComponentAnimationAction::Apply(
	GSceneNode& in_sceneNode,
	TPointerAnimationManagerInstance& in_animationManager,
	const float in_timeDelta
	)const
{
	switch (mType)
	{
	default:
		break;
	case TType::TSceneNodeAnimation:
		break;
	case TType::TBoneAnimation:
		ApplyVisualBoneAnimation(in_sceneNode, in_animationManager, in_timeDelta);
		break;
	case TType::TMorphTarget:
		ApplyMorphTargetAnimation(in_animationManager);
		break;
	}
	return;
}

//public Accessors
const bool GSceneNodeComponentAnimationAction::TargetIsBone(const int in_boneIndex, TPointerSkeletonInstance& in_skeletonInstance)const
{
	return ((TType::TBoneAnimation == mType) &&
			(in_boneIndex == mBoneIndex) &&
			(in_skeletonInstance == mWeakSkeletonInstance.lock()));
}

void GSceneNodeComponentAnimationAction::AddBoneUpdateCallback(
	const TBoneUpdateCallback in_boneUpdateCallback,
	void* const in_boneCallbackData
	)
{
	mBoneUpdateCallback = in_boneUpdateCallback;
	mBoneCallbackData = in_boneCallbackData;
	return;
}

//private methods;
void GSceneNodeComponentAnimationAction::ApplyVisualBoneAnimation(
	GSceneNode& in_sceneNode,
	TPointerAnimationManagerInstance& in_animationManager,
	const float in_timeDelta
	)const
{
	if (!in_animationManager)
	{
		return;
	}
	TPointerSkeletonInstance skeletonInstance = mWeakSkeletonInstance.lock();
	if (!skeletonInstance)
	{
		return;
	}

	GVector3Float at;
	GVector3Float up;
	GVector3Float pos;
	GMatrix16FloatDecomposeAtUp(
		at,
		up,
		pos,
		skeletonInstance->GetOriginalParentRelativeBoneMatrix(mBoneIndex)
		);
	float scale = Orthogonalise(at, up);

	if (GCOMMON_INVALID_INDEX != mStreamIndexPosition)
	{
		in_animationManager->ApplyToStreamPosition(
			pos,
			mStreamIndexPosition
			);
	}

	if (GCOMMON_INVALID_INDEX != mStreamIndexRotation)
	{
		in_animationManager->ApplyToStreamRotation(
			at,
			up,
			mStreamIndexRotation
			);
	}

	if (GCOMMON_INVALID_INDEX != mStreamIndexScale)
	{
		in_animationManager->ApplyToStreamScale(
			scale,
			mStreamIndexScale
			);
	}

	//apply bone callback
	if (mBoneUpdateCallback)
	{
		(*mBoneUpdateCallback)(
			at,
			up,
			pos,
			scale,
			in_sceneNode,
			in_timeDelta,
			mBoneCallbackData,
			*skeletonInstance
			);
	}

	Orthogonalise(at, up);

	const GMatrix16Float result = GMatrix16FloatConstructAtUp(
		at * scale,
		up * scale,
		GVector3Float::sUnitZ * scale,
		GVector3Float::sUnitY * scale,
		pos
		);

	skeletonInstance->SetParentRelativeBoneMatrix(mBoneIndex, result);

	return;
}

void GSceneNodeComponentAnimationAction::ApplyMorphTargetAnimation(TPointerAnimationManagerInstance& in_animationManager)const
{
	if (!in_animationManager)
	{
		return;
	}
	TPointerSkeletonInstance skeletonInstance = mWeakSkeletonInstance.lock();
	if (!skeletonInstance)
	{
		return;
	}

	float weight = 0.0F;
	if (GCOMMON_INVALID_INDEX != mStreamIndexPosition)
	{
		in_animationManager->ApplyToStreamMorphTargetWeight(
			weight,
			mStreamIndexPosition
			);
	}

	skeletonInstance->SetMorphTargetWeight(mBoneIndex, weight);

	return;
}
