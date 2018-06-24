//
//  GSceneNodeComponentSpringBoneLoad.cpp
//
//  Created by David Coen on 2011 01 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentSpringBoneLoad.h"

//constructor
GSceneNodeComponentSpringBoneLoad::GSceneNodeComponentSpringBoneLoad(
	const int in_arraySpringBoneDataCount,
	const GSceneNodeComponentSpringBoneDataLoad* const in_arraySpringBoneData
	)
	: mArraySpringBoneDataCount(in_arraySpringBoneDataCount)
	, mArraySpringBoneData(in_arraySpringBoneData)
{
	return;
}

GSceneNodeComponentSpringBoneLoad::~GSceneNodeComponentSpringBoneLoad()
{
	return;
}
