//
//  GSoundContext.cpp
//
//  Created by David Coen on 2011 05 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSoundContext.h"

#include "GAssert.h"
#include "GSoundBuffer.h"
#include "GSoundSource.h"
#include "GVector3Float.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

//constructor
GSoundContext::GSoundContext(
	ALCdevice* const inout_device
	)
: mContext(0)
, mArraySoundSource()
, mStaticGroupSoundSourceCount(0)
{
	mContext = alcCreateContext(inout_device, NULL);
	alcMakeContextCurrent(mContext);
	alcProcessContext(mContext);
	alDistanceModel(AL_LINEAR_DISTANCE_CLAMPED);
	GASSERT(AL_NO_ERROR == alGetError(), "problem with audio"); 

	return;
}

GSoundContext::~GSoundContext()
{
	alcDestroyContext(mContext);	

	return;
}

//public methods
void GSoundContext::CreateSoundSourceStaticGroup(
	const TArrayInt& in_arrayBufferIndex,
	TArrayPointerSoundBuffer& inout_arraySoundBuffer
	)
{
	mStaticGroupSoundSourceCount = (int)in_arrayBufferIndex.size();
	mArraySoundSource.resize(mStaticGroupSoundSourceCount);
	for (int index = 0; index < mStaticGroupSoundSourceCount; ++index)
	{
		const int bufferIndex = in_arrayBufferIndex[index];
		if ((bufferIndex < 0) ||
			((int)inout_arraySoundBuffer.size() <= bufferIndex))
		{
			continue;
		}
		TPointerSoundBuffer& pointerBuffer = inout_arraySoundBuffer[bufferIndex];
		if (!pointerBuffer)
		{
			continue;
		}

		mArraySoundSource[index].reset(new GSoundSource(pointerBuffer->GetBufferId()));
	}
}

void GSoundContext::CreateSoundSourceDynamicGroup(
	const TArrayInt& in_arrayBufferIndex,
	TArrayPointerSoundBuffer& inout_arraySoundBuffer
	)
{
	mArraySoundSource.resize(mStaticGroupSoundSourceCount);

	const int count = in_arrayBufferIndex.size();
	mArraySoundSource.resize(mStaticGroupSoundSourceCount + count);
	for (int index = 0; index < count; ++index)
	{
		const int bufferIndex = in_arrayBufferIndex[index];
		if ((bufferIndex < 0) ||
			((int)inout_arraySoundBuffer.size() <= bufferIndex))
		{
			continue;
		}
		TPointerSoundBuffer& pointerBuffer = inout_arraySoundBuffer[bufferIndex];
		if (!pointerBuffer)
		{
			continue;
		}

		mArraySoundSource[mStaticGroupSoundSourceCount + index].reset(new GSoundSource(pointerBuffer->GetBufferId()));
	}

	return;
}

//public accessors
GSoundContext::TWeakSoundSource GSoundContext::GetSoundSource(const int in_soundSourceIndex)
{
	if ((0 <= in_soundSourceIndex) &&
		(in_soundSourceIndex < (int)mArraySoundSource.size()))
	{
		return mArraySoundSource[in_soundSourceIndex];
	}
	return TWeakSoundSource();
}

void GSoundContext::SetVolume(const float in_volume)
{
	alcMakeContextCurrent(mContext);
	alListenerf(AL_GAIN, in_volume);		
	return;
}

void GSoundContext::SetListenerPosition(
	const GVector3Float& in_position,
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const GVector3Float& in_velocity
	)
{
	alcMakeContextCurrent(mContext);
	alListenerfv(AL_POSITION, in_position.GetData());
	alListenerfv(AL_VELOCITY, in_velocity.GetData());
	ALfloat orientation[6];
	orientation[0] = in_at.m_x;
	orientation[1] = in_at.m_y;
	orientation[2] = in_at.m_z;
	orientation[3] = in_up.m_x;
	orientation[4] = in_up.m_y;
	orientation[5] = in_up.m_z;
	alListenerfv(AL_ORIENTATION, &orientation[0]);
	return;
}
