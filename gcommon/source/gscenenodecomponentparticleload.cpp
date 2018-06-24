//
//  GSceneNodeComponentParticleLoad.cpp
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentParticleLoad.h"

#include "GSceneNodeLoad.h"
#include "GAssert.h"

//constructor
GSceneNodeComponentParticleLoad::GSceneNodeComponentParticleLoad(
	const char* const in_data,
	const char* const in_materialName,
	const float in_radius,
	const int in_defaultEmittor,
	const float in_defaultEmittorDuration,
	const int in_maxParticleCount,
	const int in_maxLineCount,
	const int in_maxEmitorCount
	)
	: mData(in_data)
	, mMaterialName(in_materialName)
	, mRadius(in_radius)
	, mDefaultEmittor(in_defaultEmittor)
	, mDefaultEmittorDuration(in_defaultEmittorDuration)
	, mMaxParticleCount(in_maxParticleCount)
	, mMaxLineCount(in_maxLineCount)
	, mMaxEmittorCount(in_maxEmitorCount)
{
	return;
}

GSceneNodeComponentParticleLoad::~GSceneNodeComponentParticleLoad()
{
	return;
}
