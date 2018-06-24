//
//  GSoundManager.h
//
//  Created by David Coen on 2011 03 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSoundManager_h_
#define _GSoundManager_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>
#include "GCommon.h"

class GFileSystem;
class GSoundBuffer;
class GSoundContext;
class GSoundSource;
class GSoundManagerLoad;
class GVector3Float;
typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

/**/
class GSoundManager : public boost::noncopyable 
{
	//typedef
private:
	typedef boost::shared_ptr<GSoundSource> TPointerSoundSource;
	typedef boost::weak_ptr<GSoundSource> TWeakSoundSource;

	typedef boost::shared_ptr<GSoundContext> TPointerSoundContext;
	typedef boost::weak_ptr<GSoundContext> TWeakSoundContext;
	typedef std::vector<TPointerSoundContext> TArrayPointerSoundContext;

	typedef boost::shared_ptr<GSoundBuffer> TPointerSoundBuffer;
	typedef std::vector<TPointerSoundBuffer> TArrayPointerSoundBuffer;

	typedef std::vector<int> TArrayInt;

	//constructor
public:
	GSoundManager(
		GFileSystem& inout_fileSystem,
		const GSoundManagerLoad& in_soundManagerLoad,
		const TArrayInt& in_arrayBufferIndexStaticGroup //sound sources which are perminant
		);
	~GSoundManager();

	//public methods
public:
	void CreateSoundSourceDynamicGroup(
		const TArrayInt& in_arrayBufferIndexDynamicGroup //sound sources which are not perminant
		);

	TWeakSoundSource GetSoundSource(const int in_soundSourceIndex);

	void SetVolume(const float in_volume);
	void SetListenerPosition(
		const GVector3Float& in_position,
		const GVector3Float& in_at,
		const GVector3Float& in_up,
		const GVector3Float& in_velocity
		);

	//private members;
private:
	ALCdevice* mDevice;
	//argh, win32 allows only one sound context
	////ALCcontext* mBufferContext; //doh, need a context to be able to load buffers
	////TArrayPointerSoundContext mArraySoundContext;
	TPointerSoundContext mSoundContext;
	TArrayPointerSoundBuffer mArraySoundBuffer;

};

#endif 