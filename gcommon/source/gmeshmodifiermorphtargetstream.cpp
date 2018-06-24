//
//  GMeshModifierMorphTargetStream.cpp
//
//  Created by David Coen on 2011 03 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierMorphTargetStream.h"

//constructor
GMeshModifierMorphTargetStream::GMeshModifierMorphTargetStream(
	const void* const in_streamData,
	const int in_arrayDuplicateTableOffsetCount,
	const int* const in_arrayDuplicateTableOffset
	)
	: mStreamData(in_streamData)
	, mArrayDuplicateTableOffsetCount(in_arrayDuplicateTableOffsetCount)
	, mArrayDuplicateTableOffset(in_arrayDuplicateTableOffset)
{
	return;
}

GMeshModifierMorphTargetStream::~GMeshModifierMorphTargetStream()
{
	return;
}
