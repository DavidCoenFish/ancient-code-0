//
//  GSceneNodeComponentAnimationLoad.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentAnimationLoad.h"

//constructor
GSceneNodeComponentAnimationLoad::GSceneNodeComponentAnimationLoad(
	const char* const in_animationAssetFileName,
	const char* const in_defaultAnimationName
	)
	: mAnimationAssetFileName(in_animationAssetFileName)
	, mDefaultAnimationName(in_defaultAnimationName)
{
	return;
}

GSceneNodeComponentAnimationLoad::~GSceneNodeComponentAnimationLoad()
{
	return;
}
