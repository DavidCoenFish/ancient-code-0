//
//  GMeshModifierCloth.cpp
//
//  Created by David Coen on 2011 01 03
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierCloth.h"

#include "GMeshModifierSkinBone.h"
#include "GMeshModifierSkinTargetStream.h"

//constructor
GMeshModifierCloth::GMeshModifierCloth(
	const float in_gravity,
	const float in_mass,
	const float in_springConstant,
	const float in_springDampen,
	const unsigned int in_springDataArrayCount,
	const GMeshModifierClothSpringData* const in_springDataLoadArray,
	const unsigned int in_vertexDataArrayCount,
	const GMeshModifierClothVertexData* const in_vertexDataLoadArray,
	const unsigned int* const in_vectorArrayDuplicate,
	const unsigned int in_streamTargetArrayCount,
	const unsigned int* const in_streamTargetArray
	)
	: mGravity(in_gravity)
	, mMass(in_mass)
	, mSpringConstant(in_springConstant)
	, mSpringDampen(in_springDampen)
	, mSpringDataArrayCount(in_springDataArrayCount)
	, mSpringDataLoadArray(in_springDataLoadArray)
	, mVertexDataArrayCount(in_vertexDataArrayCount)
	, mVertexDataLoadArray(in_vertexDataLoadArray)
	, mVectorArrayDuplicate(in_vectorArrayDuplicate)
	, mStreamTargetArrayCount(in_streamTargetArrayCount)
	, mStreamTargetArray(in_streamTargetArray)
{
	return;
}

GMeshModifierCloth::~GMeshModifierCloth()
{
	return;
}
