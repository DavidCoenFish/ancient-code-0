//
//  GAnimationStreamDataLoad.cpp
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GAnimationStreamDataLoad.h"

//constructor
GAnimationStreamDataLoad::GAnimationStreamDataLoad(
	const unsigned int in_arrayKeyCount,
	const GAnimationKeyLoad* const in_arrayKeyLoad
	)
	: mArrayKeyCount(in_arrayKeyCount)
	, mArrayKeyLoad(in_arrayKeyLoad)
{
	return;
}

GAnimationStreamDataLoad::~GAnimationStreamDataLoad()
{
	return;
}

