//
//  GSoundSource.cpp
//
//  Created by David Coen on 2011 05 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSoundSource.h"
#include "GVector3Float.h"
#include "GAssert.h"
#include "GSoundUtility.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

//constructor
GSoundSource::GSoundSource(
	const unsigned int in_bufferId
	)
	: mSourceId(0)
{
	AL_ERROR_CHECK();

	alGenSources(1, &mSourceId);
	alSourcei(mSourceId, AL_BUFFER, in_bufferId);
	
	alSourcef(mSourceId, AL_PITCH, 1.0F);
	alSourcef(mSourceId, AL_GAIN, 1.0F);
	alSourcef(mSourceId, AL_MAX_DISTANCE, 10.0F);
	alSourcef(mSourceId, AL_REFERENCE_DISTANCE, 1.0F);
		
	AL_ERROR_CHECK();

	return;
}

GSoundSource::~GSoundSource()
{	
	AL_ERROR_CHECK();
	alDeleteSources(1, &mSourceId);
	AL_ERROR_CHECK();
	return;
}

//public methods
void GSoundSource::Play()
{
	AL_ERROR_CHECK();
	alSourcePlay(mSourceId);
	AL_ERROR_CHECK();
	return;
}

void GSoundSource::Pause()
{
	AL_ERROR_CHECK();
	alSourcePause(mSourceId);
	AL_ERROR_CHECK();
	return;
}

void GSoundSource::Stop()
{
	AL_ERROR_CHECK();
	alSourceStop(mSourceId);
	AL_ERROR_CHECK();
	return;
}

//public accessors
void GSoundSource::SetVolume(const float in_volume)
{
	return;
}

void GSoundSource::SetPan(const float in_pan)
{
	return;
}

const bool GSoundSource::GetIsPlaying()const
{
	AL_ERROR_CHECK();
	ALenum state;
	alGetSourcei(mSourceId, AL_SOURCE_STATE, &state);
	
	AL_ERROR_CHECK();
	return (state == AL_PLAYING);
}

void GSoundSource::SetPosition(const GVector3Float& in_position)
{
	AL_ERROR_CHECK();
	alSourcefv(mSourceId, AL_POSITION, in_position.GetData());
	AL_ERROR_CHECK();
	return;
}

void GSoundSource::SetOmni(const bool in_omni)
{
	AL_ERROR_CHECK();
//AL_API void AL_APIENTRY alGetListenerfv( ALenum param, ALfloat* values );

	if (in_omni == true)
	{
		alSourcef(mSourceId, AL_MAX_DISTANCE, 10000.0F);
		alSourcef(mSourceId, AL_REFERENCE_DISTANCE, 1000.0F);
	}
	else
	{
		alSourcef(mSourceId, AL_MAX_DISTANCE, 10.0F);
		alSourcef(mSourceId, AL_REFERENCE_DISTANCE, 1.0F);
	}
	AL_ERROR_CHECK();
	return;
}

void GSoundSource::SetLoop(const bool in_loop)
{
	AL_ERROR_CHECK();

	if (in_loop == true)
	{
		alSourcei(mSourceId, AL_LOOPING, AL_TRUE);
	}
	else
	{
		alSourcei(mSourceId, AL_LOOPING, AL_FALSE);
	}

	AL_ERROR_CHECK();
	return;
}


