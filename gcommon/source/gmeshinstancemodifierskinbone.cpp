//
//  GMeshInstanceModifierSkinBone.cpp
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshInstanceModifierSkinBone.h"

#include "GMeshModifierSkin.h"
#include "GMeshModifierSkinBone.h"
#include "GAssert.h"

//constructor
GMeshInstanceModifierSkinBone::GMeshInstanceModifierSkinBone(
	const GMatrix16Float& in_parentRelativeTransform
	)
: mParentRelativeTransform(in_parentRelativeTransform)
, mObjectSpaceTransform()
, mObjectSpaceTransformDirty(true)
{
	return;
}

GMeshInstanceModifierSkinBone::~GMeshInstanceModifierSkinBone()
{
	return;
}

//public methods
const bool GMeshInstanceModifierSkinBone::SetParentRelativeTransform(const GMatrix16Float& in_parentRelativeTransform)
{
	mParentRelativeTransform = in_parentRelativeTransform;
	return MarkObjectSpaceTransformDirty();
}

const bool GMeshInstanceModifierSkinBone::MarkObjectSpaceTransformDirty()
{
	if (false == mObjectSpaceTransformDirty)
	{
		mObjectSpaceTransformDirty = true;
		return true;
	}
	return false;
}

//public accessors
const GMatrix16Float& GMeshInstanceModifierSkinBone::GetObjectSpaceTransform(
	const int in_boneIndex,
	TArrayPointerRenderMeshInstanceModifierSkinBone& inout_arrayInstanceSkinBone,
	const GMeshModifierSkin& in_modifierSkinLoad
	)
{
	if (mObjectSpaceTransformDirty)
	{
		mObjectSpaceTransformDirty = false;
		GASSERT(false, "TODO");
		const int parentBoneIndex = -1; //in_modifierSkinLoad.GetBone(in_boneIndex).GetParentIndex();
		if (-1 == parentBoneIndex)
		{
			mObjectSpaceTransform = mParentRelativeTransform;
		}
		else
		{
			const GMatrix16Float& parentObjectTransform = inout_arrayInstanceSkinBone[parentBoneIndex]->GetObjectSpaceTransform(parentBoneIndex, inout_arrayInstanceSkinBone, in_modifierSkinLoad);
			mObjectSpaceTransform = mParentRelativeTransform * parentObjectTransform;
		}
	}

	return mObjectSpaceTransform;
}
