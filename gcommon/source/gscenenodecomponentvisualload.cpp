//
//  GSceneNodeComponentVisualLoad.cpp
//
//  Created by David Coen on 2010 12 25
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentVisualLoad.h"

#include "GSceneNodeLoad.h"
#include "GAssert.h"

//constructor
GSceneNodeComponentVisualLoad::GSceneNodeComponentVisualLoad(
	const char* const in_meshName,
	const char* const in_materialName,
	const float in_radius,
	const char* const in_data,
	const int in_flag
	)
	: mMeshName(in_meshName)
	, mMaterialName(in_materialName)
	, mRadius(in_radius)
	, mData(in_data)
	, mFlag(in_flag)
{
	return;
}

GSceneNodeComponentVisualLoad::~GSceneNodeComponentVisualLoad()
{
	return;
}
