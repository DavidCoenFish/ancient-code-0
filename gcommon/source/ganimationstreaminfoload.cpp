//
//  GAnimationStreamInfoLoad.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationStreamInfoLoad.h"

//constructor
GAnimationStreamInfoLoad::GAnimationStreamInfoLoad(
	const char* const in_targetName, 
	const int in_targetType
	)
	: mTargetName(in_targetName)
	, mTargetType(in_targetType)
{
	return;
}

GAnimationStreamInfoLoad::~GAnimationStreamInfoLoad()
{
	return;
}

