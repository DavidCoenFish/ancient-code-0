//
//  GSkeletonBone.cpp
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSkeletonBone.h"

#include "GSkeletonLoad.h"
#include "GSkeletonBoneLoad.h"

//constructor
GSkeletonBone::GSkeletonBone(
	const GMatrix16Float& in_parentRelativeTransform
	)
: mParentRelativeTransform(in_parentRelativeTransform)
, mObjectSpaceTransform()
, mObjectSpaceTransformDirty(true)
{
	return;
}

GSkeletonBone::~GSkeletonBone()
{
	return;
}

//public methods
const bool GSkeletonBone::SetParentRelativeTransform(const GMatrix16Float& in_parentRelativeTransform)
{
	mParentRelativeTransform = in_parentRelativeTransform;
	return MarkObjectSpaceTransformDirty();
}

const bool GSkeletonBone::MarkObjectSpaceTransformDirty()
{
	if (false == mObjectSpaceTransformDirty)
	{
		mObjectSpaceTransformDirty = true;
		return true;
	}
	return false;
}

//public accessors
const GMatrix16Float& GSkeletonBone::GetObjectSpaceTransform(
	const int in_boneIndex,
	TArrayPointerSkeletonBone& inout_arraySkeletonBone,
	const GSkeletonLoad& in_skeletonLoad
	)
{
	if (mObjectSpaceTransformDirty)
	{
		mObjectSpaceTransformDirty = false;
		const int parentBoneIndex = in_skeletonLoad.GetArraySkeletonBoneLoad()[in_boneIndex].GetParentIndex();
		if (-1 == parentBoneIndex)
		{
			mObjectSpaceTransform = mParentRelativeTransform;
		}
		else
		{
			const GMatrix16Float& parentObjectTransform = inout_arraySkeletonBone[parentBoneIndex]->GetObjectSpaceTransform(
				parentBoneIndex, 
				inout_arraySkeletonBone, 
				in_skeletonLoad
				);
			mObjectSpaceTransform = mParentRelativeTransform * parentObjectTransform;
		}
	}

	return mObjectSpaceTransform;
}
