//
//  GMeshModifierSkin.cpp
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierSkin.h"

#include "GMeshModifierSkinBone.h"
#include "GMeshModifierSkinTargetStream.h"

//constructor
GMeshModifierSkin::GMeshModifierSkin(
	const unsigned int in_arrayBoneCount,
	const GMeshModifierSkinBone* const in_arrayBone,
	const unsigned int in_arrayTargetStreamCount,
	const GMeshModifierSkinTargetStream* const in_arrayTargetStreamLoad,
	const unsigned int in_pointArrayCount,
	const unsigned int* const in_pointArrayPairIndexDuplicateOffset,
	const unsigned int* const in_pointArrayDuplicate,
	const unsigned int in_vectorArrayCount,
	const unsigned int* const in_vectorArrayPairIndexDuplicateOffset,
	const unsigned int* const in_vectorArrayDuplicate
	)
	: mArrayBoneCount(in_arrayBoneCount)
	, mArrayBone(in_arrayBone)
	, mArrayTargetStreamCount(in_arrayTargetStreamCount)
	, mArrayTargetStreamLoad(in_arrayTargetStreamLoad)
	, mPointArrayCount(in_pointArrayCount)
	, mPointArrayPairIndexDuplicateOffset(in_pointArrayPairIndexDuplicateOffset)
	, mPointArrayDuplicate(in_pointArrayDuplicate)
	, mVectorArrayCount(in_vectorArrayCount)
	, mVectorArrayPairIndexDuplicateOffset(in_vectorArrayPairIndexDuplicateOffset)
	, mVectorArrayDuplicate(in_vectorArrayDuplicate)
{
	return;
}

GMeshModifierSkin::~GMeshModifierSkin()
{
	return;
}

//public accessors
const GMeshModifierSkinBone& GMeshModifierSkin::GetBone(const int in_index)const
{
	return mArrayBone[in_index];
}

const GMeshModifierSkinTargetStream& GMeshModifierSkin::GetArrayTargetStreamLoad(const int in_index)const
{
	return mArrayTargetStreamLoad[in_index];
}


