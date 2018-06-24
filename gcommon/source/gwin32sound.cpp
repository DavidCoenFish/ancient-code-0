//
//  GWin32Sound.cpp
//
//  Created by David Coen on 2011 03 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GWin32Sound.h"

#include "GAssert.h"
#include "GBuffer.h"
#include "GFileSystem.h"
#include "GBufferUtility.h"
#include "GSoundUtility.h"

#include <windows.h>
#include <stdio.h>

#include <OpenAL/al.h>
#include <OpenAL/alc.h>


struct TWaveFileHeader
{
	const char			szRIFF[4];
	const unsigned long	ulRIFFSize;
	const char			szWAVE[4];
};

struct TRiffChunk
{
	const char			szChunkName[4];
	const unsigned long	ulChunkSize;
};

struct TFormatChunk //"fmt "
{
	const short mAudioFormat;
	const short mChannels;
	const int mSampleRate;
	const int mByteRate;
	const short mBlockAlign;
	const short mBitsPerSample;
};

static const bool LocalCheckChunkName(
	const char* const in_tag,
	const char* const in_chunkName
	)
{
	for (int index = 0; index < 4; ++index)
	{
		if (in_tag[index] != in_chunkName[index])
		{
			return false;
		}
	}
	return true;
}

static const bool LocalLoadWaveFile(
	ALenum& out_eBufferFormat,
	ALchar*& out_pData,
	ALint& out_iDataSize,
	ALint& out_iFrequency,
	GBuffer& in_fileBuffer
	)
{
	const TWaveFileHeader* const pWaveFileHeader = (TWaveFileHeader*)in_fileBuffer.GetData();
	int trace = 0;

	if (!LocalCheckChunkName("RIFF", pWaveFileHeader->szRIFF) ||
		!LocalCheckChunkName("WAVE", pWaveFileHeader->szWAVE))
	{
		return false;
	}
	trace += sizeof(TWaveFileHeader);

	const TRiffChunk* const pBlock = (TRiffChunk*)(in_fileBuffer.GetData() + trace);
	if (!LocalCheckChunkName("fmt ", pBlock->szChunkName))
	{
		return false;
	}

	trace += sizeof(TRiffChunk);

	const TFormatChunk* const pFmtChunk = (TFormatChunk*)(in_fileBuffer.GetData() + trace);
	trace += pBlock->ulChunkSize;

	GASSERT(1 == pFmtChunk->mAudioFormat, "PCM wave files only, mono or stereo, 8 or 16bit");

	out_iFrequency = pFmtChunk->mSampleRate;

	out_eBufferFormat = 0;
	switch (pFmtChunk->mBitsPerSample)
	{
	default:
		break;
	case 8:
		if (1 == pFmtChunk->mChannels)
		{	
			out_eBufferFormat = AL_FORMAT_MONO8;
		}
		else if (2 == pFmtChunk->mChannels)
		{
			out_eBufferFormat = AL_FORMAT_STEREO8;
		}
		break;
	case 16:
		if (1 == pFmtChunk->mChannels)
		{	
			out_eBufferFormat = AL_FORMAT_MONO16;
		}
		else if (2 == pFmtChunk->mChannels)
		{
			out_eBufferFormat = AL_FORMAT_STEREO16;
		}
		break;
	}
	GASSERT(0 != out_eBufferFormat, "unsupported wav file sample rate, 8 or 16, mono or stero");

	while (trace < in_fileBuffer.GetCount())
	{
		const TRiffChunk* const pRiffChunk = (TRiffChunk*)(in_fileBuffer.GetData() + trace);
		trace += sizeof(TRiffChunk);
		if (LocalCheckChunkName("data", pRiffChunk->szChunkName))
		{
			out_iDataSize = pRiffChunk->ulChunkSize;
			out_pData = (ALchar *)(in_fileBuffer.GetData() + trace);
		}

		trace += pRiffChunk->ulChunkSize;
	}

	return true;
}

//static public methods
/*static*/ const unsigned int GWin32Sound::LoadSound(
	GFileSystem& inout_fileSystem,
	const std::string& in_resourceName
	)
{
	AL_ERROR_CHECK();

	GBuffer fileBuffer;
	if (!inout_fileSystem.LoadFile(fileBuffer, in_resourceName))
	{
		return 0;
	}

	ALint iDataSize = 0;
	ALint iFrequency = 0;
	ALenum eBufferFormat = 0;
	ALchar* pData = 0;
	ALuint bufferId = 0;

	if (LocalLoadWaveFile(
			eBufferFormat,
			pData,
			iDataSize,
			iFrequency,
			fileBuffer
			))
	{
		alGetError();
		alGenBuffers(1, &bufferId);
		alBufferData(bufferId, eBufferFormat, pData, iDataSize, iFrequency);

		//GASSERT(AL_NO_ERROR == alGetError(), "problem with sound"); 
		AL_ERROR_CHECK();
	}

	return bufferId;
}
