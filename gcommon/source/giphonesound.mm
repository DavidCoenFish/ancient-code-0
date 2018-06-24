//
//  GIPhoneSound.cpp
//
//  Created by David Coen on 2011 03 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GIPhoneSound.h"

#include "gassert.h"

#include <openal/al.h>
#include <openal/alc.h>
#include <foundation/foundation.h>
#include <UIKit/UIKit.h>
#include <AVFoundation/AVFoundation.h>
#include <AudioToolbox/AudioToolbox.h>
#include <AudioToolbox/ExtendedAudioFile.h>

static ALvoid* LocalGetOpenAlAudioData(
	CFURLRef in_filePath, 
	ALsizei& out_size, 
	ALenum& out_format, 
	ALsizei& out_freq
	)
{
	OSStatus err = noErr;
	SInt64 theFileLengthInFrames = 0;
	AudioStreamBasicDescription theFileFormat;
	UInt32 thePropertySize = sizeof(theFileFormat);
	ExtAudioFileRef extRef = NULL;
	void* theData = NULL;
	AudioStreamBasicDescription theOutputFormat;
	
	err = ExtAudioFileOpenURL(in_filePath, &extRef);
	GASSERT(!err, "open sound url failed");
	
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileDataFormat, &thePropertySize, &theFileFormat);
	GASSERT(!err, "get property failed");
	GASSERT(theFileFormat.mChannelsPerFrame <= 2, "too many channels");
	
	theOutputFormat.mSampleRate = theFileFormat.mSampleRate;
	theOutputFormat.mChannelsPerFrame = theFileFormat.mChannelsPerFrame;

	theOutputFormat.mFormatID = kAudioFormatLinearPCM;
	theOutputFormat.mBytesPerPacket = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mFramesPerPacket = 1;
	theOutputFormat.mBytesPerFrame = 2 * theOutputFormat.mChannelsPerFrame;
	theOutputFormat.mBitsPerChannel = 16;
	theOutputFormat.mFormatFlags = kAudioFormatFlagsNativeEndian | kAudioFormatFlagIsPacked | kAudioFormatFlagIsSignedInteger;

	err = ExtAudioFileSetProperty(extRef, kExtAudioFileProperty_ClientDataFormat, sizeof(theOutputFormat), &theOutputFormat);
	GASSERT(!err, "set property failed");
 
	// Get the total frame count
	thePropertySize = sizeof(theFileLengthInFrames);
	err = ExtAudioFileGetProperty(extRef, kExtAudioFileProperty_FileLengthFrames, &thePropertySize, &theFileLengthInFrames);
	GASSERT(!err, "get property failed");

	// Read all the data into memory
	UInt32		dataSize = theFileLengthInFrames * theOutputFormat.mBytesPerFrame;;
	theData = malloc(dataSize);
	if (theData)
	{
		AudioBufferList		theDataBuffer;
		theDataBuffer.mNumberBuffers = 1;
		theDataBuffer.mBuffers[0].mDataByteSize = dataSize;
		theDataBuffer.mBuffers[0].mNumberChannels = theOutputFormat.mChannelsPerFrame;
		theDataBuffer.mBuffers[0].mData = theData;
 
		// Read the data into an AudioBufferList
		err = ExtAudioFileRead(extRef, (UInt32*)&theFileLengthInFrames, &theDataBuffer);
		GASSERT(!err, "file read failed");

		out_size = (ALsizei)dataSize;
		out_format = (theOutputFormat.mChannelsPerFrame > 1) ? AL_FORMAT_STEREO16 : AL_FORMAT_MONO16;
		out_freq = (ALsizei)theOutputFormat.mSampleRate;
	}
 
	if (extRef)
	{
		ExtAudioFileDispose(extRef);
	}

	return theData;
}

//static public methods
/*static*/ const unsigned int GIPhoneSound::LoadSound(const std::string& in_resourceName)
{
	ALvoid* outData;
	//ALenum error = AL_NO_ERROR;
	ALenum format;
	ALsizei size;
	ALsizei freq;
	
	NSString *path = [NSString stringWithFormat:@"%@%@",
		[[NSBundle mainBundle] resourcePath],
		[[[NSString alloc] initWithUTF8String:in_resourceName.c_str()] autorelease]
		];
	CFURLRef filePath = (CFURLRef)[NSURL fileURLWithPath:path isDirectory:NO];

	outData = LocalGetOpenAlAudioData(filePath, size, format, freq);	
	
	alGetError();
	ALuint bufferId = 0;
	alGenBuffers(1, &bufferId);
	alBufferData(bufferId, format, outData, size, freq);

	GASSERT(AL_NO_ERROR == alGetError(), "problem with sound"); 

	if (outData)
	{
		free(outData);
	}

	return bufferId;
}
	