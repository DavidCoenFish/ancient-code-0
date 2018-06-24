//
//  GSoundBuffer.cpp
//
//  Created by David Coen on 2011 05 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSoundBuffer.h"
#include "GASSERT.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

#ifdef IPHONE
#include "GIPhoneSound.h"
#else
#include "GWin32Sound.h"
#endif

//constructor
GSoundBuffer::GSoundBuffer(
	GFileSystem& inout_fileSystem,
	const std::string& in_resourceName
	)
	: mBufferId(0)
{
#ifdef IPHONE
	mBufferId = GIPhoneSound::LoadSound(in_resourceName);
#else
	mBufferId = GWin32Sound::LoadSound(inout_fileSystem, in_resourceName);
#endif

	GASSERT(0 != mBufferId, "sound buffer fail");

	return;
}

GSoundBuffer::~GSoundBuffer()
{
	alDeleteBuffers(1, &mBufferId);
	return;
}
