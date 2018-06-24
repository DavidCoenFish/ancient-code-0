//
//  GSceneNodeComponentNodeAttachLoad.cpp
//
//  Created by David Coen on 2011 01 04
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentNodeAttachLoad.h"

//constructor
GSceneNodeComponentNodeAttachLoad::GSceneNodeComponentNodeAttachLoad(
	const char* const in_boneName,
	const char* const in_name,
	const GMatrix16Float& in_transformParentRelative,
	const int in_componentCount,
	const GSceneNodeComponentLoad* const in_arrayComponentLoad,
	const int in_childCount,
	const GSceneNodeLoad* const in_arrayChild
	)
	: mBoneName(in_boneName)
	, mSceneNodeLoad(
		in_name,
		&in_transformParentRelative,
		in_componentCount,
		in_arrayComponentLoad,
		in_childCount,
		in_arrayChild
		)
{
	return;
}

GSceneNodeComponentNodeAttachLoad::~GSceneNodeComponentNodeAttachLoad()
{
	return;
}

//public accessors
const GMatrix16Float& GSceneNodeComponentNodeAttachLoad::GetTransformParentRelative()const
{
	return mSceneNodeLoad.GetTransformParentRelative();
}
