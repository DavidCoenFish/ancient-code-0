//
//  GAnimationManagerLoad.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationManagerLoad.h"

//constructor
GAnimationManagerLoad::GAnimationManagerLoad(
	const unsigned int in_arrayStreamInfoCount,
	const GAnimationStreamInfoLoad* const in_arrayStreamInfo,
	const unsigned int in_arrayAnimationCount,
	const GAnimationLoad* const in_arrayAnimation
	)
	: mArrayStreamInfoCount(in_arrayStreamInfoCount)
	, mArrayStreamInfo(in_arrayStreamInfo)
	, mArrayAnimationCount(in_arrayAnimationCount)
	, mArrayAnimation(in_arrayAnimation)
{
	return;
}

GAnimationManagerLoad::~GAnimationManagerLoad()
{
	return;
}
