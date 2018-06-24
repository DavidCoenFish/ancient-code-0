//
//  GMeshModifierSkinBoneVertexData.cpp
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierSkinBoneVertexData.h"

//constructor
GMeshModifierSkinBoneVertexData::GMeshModifierSkinBoneVertexData(
	const unsigned int in_vertexTargetIndex,
	const float in_weight
	)
	: mVertexTargetIndex(in_vertexTargetIndex)
	, mWeight(in_weight)
{
	return;
}

GMeshModifierSkinBoneVertexData::~GMeshModifierSkinBoneVertexData()
{
	return;
}
