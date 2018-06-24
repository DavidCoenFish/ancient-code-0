//
//  GAnimationLoad.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationLoad.h"

//constructor
GAnimationLoad::GAnimationLoad(
	const char* const in_name,
	const float in_length,
	const int in_flag,
	const GAnimationStreamDataLoad* const in_arrayStreamDataLoad
	)
	: mName(in_name)
	, mLength(in_length)
	, mFlag(in_flag)
	, mArrayStreamDataLoad(in_arrayStreamDataLoad)
{
	return;
}

GAnimationLoad::~GAnimationLoad()
{
	return;
}

