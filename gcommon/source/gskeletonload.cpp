//
//  GSkeletonLoad.cpp
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSkeletonLoad.h"

//constructor
GSkeletonLoad::GSkeletonLoad(
	const int in_boneCount,
	const GSkeletonBoneLoad* const in_arraySkeletonBoneLoad,
	const int in_morphTargetCount,
	const GSkeletonMorphTargetLoad* const in_arraySkeletonMorphTargetLoad
	)
	: mBoneCount(in_boneCount)
	, mArraySkeletonBoneLoad(in_arraySkeletonBoneLoad)
	, mMorphTargetCount(in_morphTargetCount)
	, mArraySkeletonMorphTargetLoad(in_arraySkeletonMorphTargetLoad)
{
	return;
}

GSkeletonLoad::~GSkeletonLoad()
{
	return;
}
