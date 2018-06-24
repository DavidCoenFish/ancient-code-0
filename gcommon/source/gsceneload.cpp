//
//  GSceneLoad.cpp
//
//  Created by David Coen on 2010 12 25
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneLoad.h"

#include "GSceneNodeLoad.h"
#include "GAssert.h"

//constructor
GSceneLoad::GSceneLoad(
	const int in_sceneNodeLoadCount,
	const GSceneNodeLoad* const in_arraySceneNodeLoad
	)
	: mSceneNodeLoadCount(in_sceneNodeLoadCount)
	, mArraySceneNodeLoad(in_arraySceneNodeLoad)
{
	return;
}

GSceneLoad::~GSceneLoad()
{
	return;
}

//public accessors
const GSceneNodeLoad& GSceneLoad::GetSceneNodeLoad(const int in_index)const
{
	GASSERT((0 <= in_index) && (in_index < mSceneNodeLoadCount), "invalid param");
	return mArraySceneNodeLoad[in_index];
}
