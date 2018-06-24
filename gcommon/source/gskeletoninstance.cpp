//
//  GSkeletonInstance.cpp
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSkeletonInstance.h"

#include "GBuffer.h"
#include "GBufferUtility.h"
#include "GSkeletonBone.h"
#include "GSkeletonBoneLoad.h"
#include "GSkeletonLoad.h"
#include "GSkeletonMorphTargetLoad.h"

//constructor
GSkeletonInstance::GSkeletonInstance(TPointerBuffer& inout_assetSkeletonLoad)
: mAssetSkeletonLoad(inout_assetSkeletonLoad)
, mArrayBone()
, mArrayMorphTargetWeight()
{
	const GSkeletonLoad& skeletonLoad = GBufferUtility::GetLoadBufferRoot<GSkeletonLoad>(*inout_assetSkeletonLoad);
	const int boneCount = skeletonLoad.GetBoneCount();
	mArrayBone.resize(boneCount);
	for (int index = 0; index < boneCount; ++index)
	{
		mArrayBone[index].reset(new GSkeletonBone(
			skeletonLoad.GetArraySkeletonBoneLoad()[index].GetParentRelativeMatrix()
			));
	}

	const int morphTargetCount = skeletonLoad.GetMorphTargetCount();
	mArrayMorphTargetWeight.resize(morphTargetCount);

	return;
}

GSkeletonInstance::~GSkeletonInstance()
{
	return;
}

//public accessors
const int GSkeletonInstance::GetBoneIndex(const std::string& in_boneName)const
{
	TPointerBuffer pointerAsset = mAssetSkeletonLoad.lock();
	if (!pointerAsset)
	{
		return GCOMMON_INVALID_INDEX;
	}

	const GSkeletonLoad& skeletonLoad = GBufferUtility::GetLoadBufferRoot<GSkeletonLoad>(*pointerAsset);
	for (int boneIndex = 0; boneIndex < skeletonLoad.GetBoneCount(); ++boneIndex )
	{
		if (in_boneName == skeletonLoad.GetArraySkeletonBoneLoad()[boneIndex].GetBoneName())
		{
			return boneIndex;
		}
	}

	return GCOMMON_INVALID_INDEX;
}

const int GSkeletonInstance::GetBoneCount()const
{
	return (int)mArrayBone.size();
}

const char* const GSkeletonInstance::GetBoneName(const int in_boneIndex)const
{
	TPointerBuffer pointerAsset = mAssetSkeletonLoad.lock();
	if (!pointerAsset)
	{
		return 0;
	}

	const GSkeletonLoad& skeletonLoad = GBufferUtility::GetLoadBufferRoot<GSkeletonLoad>(*pointerAsset);
	if ((0 <= in_boneIndex) && (in_boneIndex < skeletonLoad.GetBoneCount()))
	{
		return skeletonLoad.GetArraySkeletonBoneLoad()[in_boneIndex].GetBoneName();
	}

	return 0;
}

const int GSkeletonInstance::GetBoneParentIndex(const int in_boneIndex)const
{
	TPointerBuffer pointerAsset = mAssetSkeletonLoad.lock();
	if (!pointerAsset)
	{
		return 0;
	}

	const GSkeletonLoad& skeletonLoad = GBufferUtility::GetLoadBufferRoot<GSkeletonLoad>(*pointerAsset);
	if ((0 <= in_boneIndex) && (in_boneIndex < skeletonLoad.GetBoneCount()))
	{
		return skeletonLoad.GetArraySkeletonBoneLoad()[in_boneIndex].GetParentIndex();
	}

	return GCOMMON_INVALID_INDEX;
}

const GMatrix16Float& GSkeletonInstance::GetOriginalParentRelativeBoneMatrix(const int in_boneIndex)const
{
	TPointerBuffer pointerAsset = mAssetSkeletonLoad.lock();
	if (!pointerAsset)
	{
		return GMatrix16Float::sIdentity;
	}

	const GSkeletonLoad& skeletonLoad = GBufferUtility::GetLoadBufferRoot<GSkeletonLoad>(*pointerAsset);
	GASSERT((0 <= in_boneIndex) && (in_boneIndex < skeletonLoad.GetBoneCount()), "invalid state");

	return skeletonLoad.GetArraySkeletonBoneLoad()[in_boneIndex].GetParentRelativeMatrix();
}

const GMatrix16Float& GSkeletonInstance::GetParentRelativeBoneMatrix(const int in_boneIndex)const
{
	GASSERT((0 <= in_boneIndex) && (in_boneIndex < (int)mArrayBone.size()), "invalid state");
	return mArrayBone[in_boneIndex]->GetParentRelativeTransform();
}

void GSkeletonInstance::SetParentRelativeBoneMatrix(const int in_boneIndex, const GMatrix16Float& in_parentRelativeMatrix)
{
	GASSERT((0 <= in_boneIndex) && (in_boneIndex < (int)mArrayBone.size()), "invalid state");

	if (mArrayBone[in_boneIndex]->SetParentRelativeTransform(in_parentRelativeMatrix))
	{
		//return true, need to set children dirty
		TPointerBuffer pointerAsset = mAssetSkeletonLoad.lock();
		if (!pointerAsset)
		{
			return;
		}
		const GSkeletonLoad& skeletonLoad = GBufferUtility::GetLoadBufferRoot<GSkeletonLoad>(*pointerAsset);
		MarkBoneChildDirty(in_boneIndex, skeletonLoad);
	}
	return;
}

const GMatrix16Float& GSkeletonInstance::GetBoneObjectMatrix(const int in_boneIndex)
{
	TPointerBuffer pointerAsset = mAssetSkeletonLoad.lock();
	if (!pointerAsset)
	{
		return GMatrix16Float::sIdentity;
	}

	const GSkeletonLoad& skeletonLoad = GBufferUtility::GetLoadBufferRoot<GSkeletonLoad>(*pointerAsset);

	return mArrayBone[in_boneIndex]->GetObjectSpaceTransform(
		in_boneIndex,
		mArrayBone,
		skeletonLoad
		);
}

const int GSkeletonInstance::GetMorphTargetIndex(const std::string& in_morphTargetName)const
{
	TPointerBuffer pointerAsset = mAssetSkeletonLoad.lock();
	if (!pointerAsset)
	{
		return GCOMMON_INVALID_INDEX;
	}

	const GSkeletonLoad& skeletonLoad = GBufferUtility::GetLoadBufferRoot<GSkeletonLoad>(*pointerAsset);
	for (int morphTargetIndex = 0; morphTargetIndex < skeletonLoad.GetMorphTargetCount(); ++morphTargetIndex )
	{
		if (in_morphTargetName == skeletonLoad.GetArraySkeletonMorphTargetLoad()[morphTargetIndex].GetMorphTargetName())
		{
			return morphTargetIndex;
		}
	}

	return GCOMMON_INVALID_INDEX;
}

const int GSkeletonInstance::GetMorphTargetCount()const
{
	return (int)mArrayMorphTargetWeight.size();
}

const char* const GSkeletonInstance::GetMorphTargetName(const int in_morphTargetIndex)const
{
	TPointerBuffer pointerAsset = mAssetSkeletonLoad.lock();
	if (!pointerAsset)
	{
		return 0;
	}

	const GSkeletonLoad& skeletonLoad = GBufferUtility::GetLoadBufferRoot<GSkeletonLoad>(*pointerAsset);
	if ((0 <= in_morphTargetIndex) && (in_morphTargetIndex < skeletonLoad.GetMorphTargetCount()))
	{
		return skeletonLoad.GetArraySkeletonMorphTargetLoad()[in_morphTargetIndex].GetMorphTargetName();
	}

	return 0;
}

const float GSkeletonInstance::GetMorphTargetWeight(const int in_morphTargetIndex)const
{
	if ((0 <= in_morphTargetIndex) && (in_morphTargetIndex < (int)mArrayMorphTargetWeight.size()))
	{
		return mArrayMorphTargetWeight[in_morphTargetIndex];
	}
	return 0.0F;
}

void GSkeletonInstance::SetMorphTargetWeight(const int in_morphTargetIndex, const float in_weight)
{
	if ((0 <= in_morphTargetIndex) && (in_morphTargetIndex < (int)mArrayMorphTargetWeight.size()))
	{
		mArrayMorphTargetWeight[in_morphTargetIndex] = in_weight;
	}
	return;
}


//private methods
void GSkeletonInstance::MarkBoneChildDirty(const int in_boneParentIndex, const GSkeletonLoad& in_skeletonLoad)
{
	//mark the bone that are children of in_boneParentIndex as dirty, hint, bone array is sorted so that childen are always after the parent
	GASSERT(0 <= in_boneParentIndex, "invalid state");
	const int count = in_skeletonLoad.GetBoneCount();
	for (int index = in_boneParentIndex; index < count; ++index)
	{
		const GSkeletonBoneLoad& bone = in_skeletonLoad.GetArraySkeletonBoneLoad()[index];
		if (bone.GetParentIndex() != in_boneParentIndex)
		{
			continue;
		}

		//set dirty
		if (mArrayBone[index]->MarkObjectSpaceTransformDirty())
		{
			//recurse if not already dirty
			MarkBoneChildDirty(index, in_skeletonLoad);
		}
	}

	return;
}
