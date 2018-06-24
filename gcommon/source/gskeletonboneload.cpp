//
//  GSkeletonBoneLoad.cpp
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSkeletonBoneLoad.h"

//constructor
GSkeletonBoneLoad::GSkeletonBoneLoad(
	const char* const in_boneName,
	const int in_parentIndex,
	const GMatrix16Float& in_parentRelativeMatrix
	)
	: mBoneName(in_boneName)
	, mParentIndex(in_parentIndex)
	, mParentRelativeMatrix(in_parentRelativeMatrix)
{
	return;
}

GSkeletonBoneLoad::~GSkeletonBoneLoad()
{
	return;
}
