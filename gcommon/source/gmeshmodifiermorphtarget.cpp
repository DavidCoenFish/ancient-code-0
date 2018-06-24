//
//  GMeshModifierMorphTarget.cpp
//
//  Created by David Coen on 2011 03 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierMorphTarget.h"

//constructor
GMeshModifierMorphTarget::GMeshModifierMorphTarget(
	const char* const in_targetName,
	const int in_arrayStreamCount,
	const GMeshModifierMorphTargetStream* const in_arrayStream,
	const int* const in_duplicateTable
	)
	: mTargetName(in_targetName)
	, mArrayStreamCount(in_arrayStreamCount)
	, mArrayStream(in_arrayStream)
	, mDuplicateTable(in_duplicateTable)
{
	return;
}

GMeshModifierMorphTarget::~GMeshModifierMorphTarget()
{
	return;
}
