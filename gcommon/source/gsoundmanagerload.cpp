//
//  GSoundManagerLoad.cpp
//
//  Created by David Coen on 2011 03 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSoundManagerLoad.h"

//constructor
GSoundManagerLoad::GSoundManagerLoad(
	const unsigned int in_arraySoundLoadCount,
	const GSoundLoad* const in_arraySoundLoad
	)
	: mArraySoundLoadCount(in_arraySoundLoadCount)
	, mArraySoundLoad(in_arraySoundLoad)
{
	return;
}

GSoundManagerLoad::~GSoundManagerLoad()
{
	return;
}
