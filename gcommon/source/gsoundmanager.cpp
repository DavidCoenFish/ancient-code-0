//
//  GSoundManager.cpp
//
//  Created by David Coen on 2011 03 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSoundManager.h"

#include "GBuffer.h"
#include "GSoundBuffer.h"
#include "GSoundContext.h"
#include "GSoundLoad.h"
#include "GSoundManagerLoad.h"
#include "GSoundUtility.h"
#include "gassert.h"

#include <OpenAL/al.h>
#include <OpenAL/alc.h>

//constructor
GSoundManager::GSoundManager(
	GFileSystem& inout_fileSystem,
	const GSoundManagerLoad& in_soundManagerLoad,
	const TArrayInt& in_arrayBufferIndexStaticGroup
	)
	: mDevice(0)
	, mSoundContext()
	, mArraySoundBuffer()
{
	AL_ERROR_CHECK();

	const ALCchar* defaultDevice;
	
	defaultDevice = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
	mDevice = alcOpenDevice(defaultDevice);

	AL_ERROR_CHECK();

	mSoundContext.reset(new GSoundContext(mDevice));

	const int count = in_soundManagerLoad.GetArraySoundLoadCount();
	mArraySoundBuffer.resize(count);
	for (int index = 0; index < count; ++index)
	{
		const GSoundLoad& soundLoad = in_soundManagerLoad.GetArraySoundLoad()[index];
		mArraySoundBuffer[index].reset(new GSoundBuffer(inout_fileSystem, soundLoad.GetResourceName()));
	}

	if (mSoundContext)
	{
		mSoundContext->CreateSoundSourceStaticGroup(
			in_arrayBufferIndexStaticGroup,
			mArraySoundBuffer
			);
	}

	return;
}

GSoundManager::~GSoundManager()
{
	mSoundContext.reset();
	alcCloseDevice(mDevice);

	return;
}

//public methods
void GSoundManager::CreateSoundSourceDynamicGroup(
	const TArrayInt& in_arrayBufferIndex
	)
{
	if (mSoundContext)
	{
		mSoundContext->CreateSoundSourceDynamicGroup(
			in_arrayBufferIndex,
			mArraySoundBuffer
			);
	}

	return;
}

GSoundManager::TWeakSoundSource GSoundManager::GetSoundSource(const int in_soundSourceIndex)
{
	if (mSoundContext)
	{
		return mSoundContext->GetSoundSource(in_soundSourceIndex);
	}

	return TWeakSoundSource();
}

void GSoundManager::SetVolume(const float in_volume)
{
	if (mSoundContext)
	{
		mSoundContext->SetVolume(in_volume);
	}

	return;
}

void GSoundManager::SetListenerPosition(
	const GVector3Float& in_position,
	const GVector3Float& in_at,
	const GVector3Float& in_up,
	const GVector3Float& in_velocity
	)
{
	if (mSoundContext)
	{
		mSoundContext->SetListenerPosition(
			in_position,
			in_at,
			in_up,
			in_velocity		
			);
	}

	return;
}


//GSoundManager::TWeakSoundContext GSoundManager::CreateSoundContext(
//	const TArrayInt& in_arrayBufferIndex
//	)
//{
//	TPointerSoundContext pointerSoundContext;
//
//	pointerSoundContext.reset(new GSoundContext(
//		mDevice,
//		in_arrayBufferIndex,
//		mArraySoundBuffer
//		));
//
//	mArraySoundContext.push_back(pointerSoundContext);
//
//	return pointerSoundContext;
//}
//
//void GSoundManager::DestroySoundContext(
//	TWeakSoundContext& in_soundContext
//	)
//{
//	TPointerSoundContext toErase = in_soundContext.lock();
//	if (!toErase)
//	{
//		return;
//	}
//	TArrayPointerSoundContext::iterator iterator = mArraySoundContext.begin(); 
//	while (iterator != mArraySoundContext.end())
//	{
//		if (*iterator == toErase)
//		{
//			iterator = mArraySoundContext.erase(iterator);
//		}
//		else
//		{
//			++iterator;
//		}
//	}
//
//	return;
//}