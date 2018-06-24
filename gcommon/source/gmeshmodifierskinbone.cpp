//
//  GMeshModifierSkinBone.cpp
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierSkinBone.h"

//constructor
GMeshModifierSkinBone::GMeshModifierSkinBone(
	const char* const in_boneName,
	const GMatrix16Float& in_invertBindMatrix,
	const unsigned int in_pointArrayVertexDataLoadCount,
	const GMeshModifierSkinBoneVertexData* const in_pointArrayVertexDataLoad,
	const unsigned int in_vectorArrayVertexDataLoadCount,
	const GMeshModifierSkinBoneVertexData* const in_vectorArrayVertexDataLoad
	)
	: mBoneName(in_boneName)
	, mInvertBindMatrix(in_invertBindMatrix)
	, mPointArrayVertexDataLoadCount(in_pointArrayVertexDataLoadCount)
	, mPointArrayVertexDataLoad(in_pointArrayVertexDataLoad)
	, mVectorArrayVertexDataLoadCount(in_vectorArrayVertexDataLoadCount)
	, mVectorArrayVertexDataLoad(in_vectorArrayVertexDataLoad)
{
	return;
}

GMeshModifierSkinBone::~GMeshModifierSkinBone()
{
	return;
}
