//
//  GSceneNodeLoad.cpp
//
//  Created by David Coen on 2010 12 25
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeLoad.h"

#include "GAssert.h"
#include "GSceneNodeComponentLoad.h"

//constructor
GSceneNodeLoad::GSceneNodeLoad(
	const char* const in_name,
	const GMatrix16Float* const in_transformParentRelative,
	const int in_componentCount,
	const GSceneNodeComponentLoad* const in_arrayComponentLoad,
	const int in_childCount,
	const GSceneNodeLoad* const in_arrayChild
	)
	: mName(in_name)
	, mTransformParentRelative(in_transformParentRelative)
	, mComponentCount(in_componentCount)
	, mArrayComponentLoad(in_arrayComponentLoad)
	, mChildCount(in_childCount)
	, mArrayChild(in_arrayChild)
{
	return;
}

GSceneNodeLoad::~GSceneNodeLoad()
{
	return;
}

//public accessors
const GSceneNodeComponentLoad& GSceneNodeLoad::GetComponentLoad(const int in_index)const
{
	GASSERT((0 <= in_index) && (in_index < mComponentCount), "invalid param");
	return mArrayComponentLoad[in_index];
}

const GSceneNodeLoad& GSceneNodeLoad::GetChild(const int in_index)const
{
	GASSERT((0 <= in_index) && (in_index < mChildCount), "invalid param");
	return mArrayChild[in_index];
}
