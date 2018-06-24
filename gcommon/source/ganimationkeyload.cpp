//
//  GAnimationKeyLoad.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationKeyLoad.h"

//constructor
GAnimationKeyLoad::GAnimationKeyLoad(
	const float in_time,
	const void* const in_data
	)
	: mTime(in_time)
	, mData(in_data)
{
	return;
}

GAnimationKeyLoad::~GAnimationKeyLoad()
{
	return;
}

