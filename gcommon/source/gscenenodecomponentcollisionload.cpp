//
//  GSceneNodeComponentCollisionLoad.cpp
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentCollisionLoad.h"

#include "GSceneNodeLoad.h"
#include "GAssert.h"

//constructor
GSceneNodeComponentCollisionLoad::GSceneNodeComponentCollisionLoad(
	const int in_materialId,
	const char* const in_data,
	const int in_flag,
	const int in_collisionDataCount,
	const GSceneNodeComponentCollisionDataLoad* const in_arrayCollisionData
	)
	: mMaterialId(in_materialId)
	, mData(in_data)
	, mFlag(in_flag)
	, mCollisionDataCount(in_collisionDataCount)
	, mArrayCollisionData(in_arrayCollisionData)
{
	return;
}

GSceneNodeComponentCollisionLoad::~GSceneNodeComponentCollisionLoad()
{
	return;
}
