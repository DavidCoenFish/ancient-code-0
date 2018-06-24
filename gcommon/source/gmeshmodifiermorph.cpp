//
//  GMeshModifierMorph.cpp
//
//  Created by David Coen on 2011 03 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierMorph.h"

//constructor
GMeshModifierMorph::GMeshModifierMorph(
	const unsigned int in_arrayTargetCount,
	const GMeshModifierMorphTarget* const in_arrayTarget
	)
	: mArrayTargetCount(in_arrayTargetCount)
	, mArrayTarget(in_arrayTarget)
{
	return;
}

GMeshModifierMorph::~GMeshModifierMorph()
{
	return;
}
