//
//  GMeshModifierSkinTargetStream.cpp
//
//  Created by David Coen on 2010 12 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GMeshModifierSkinTargetStream.h"

GMeshModifierSkinTargetStream::GMeshModifierSkinTargetStream(
	const unsigned int in_streamIndex,
	const unsigned int in_flag
	)
	: mStreamIndex(in_streamIndex)
	, mFlag(in_flag)
{
	return;
}

GMeshModifierSkinTargetStream::~GMeshModifierSkinTargetStream()
{
	return;
}

const bool GMeshModifierSkinTargetStream::GetIsPoint()const
{
	return (0 == mFlag);
}

const bool GMeshModifierSkinTargetStream::GetIsVector()const
{
	return (0 != mFlag);
}
