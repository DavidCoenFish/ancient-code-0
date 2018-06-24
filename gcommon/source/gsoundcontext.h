//
//  GSoundContext.h
//
//  Created by David Coen on 2011 05 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSoundContext_h_
#define _GSoundContext_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <vector>

class GSoundBuffer;
class GSoundSource;
class GVector3Float;

typedef struct ALCdevice_struct ALCdevice;
typedef struct ALCcontext_struct ALCcontext;

class GSoundContext : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GSoundSource> TPointerSoundSource;
	typedef boost::weak_ptr<GSoundSource> TWeakSoundSource;
	typedef std::vector<TPointerSoundSource> TArrayPointerSoundSource;

	typedef boost::shared_ptr<GSoundBuffer> TPointerSoundBuffer;
	typedef std::vector<TPointerSoundBuffer> TArrayPointerSoundBuffer;

	typedef std::vector<int> TArrayInt;

	//constructor
public:
	GSoundContext(
		ALCdevice* const inout_device
		);
	~GSoundContext();

	//public methods
public:
	void CreateSoundSourceStaticGroup(
		const TArrayInt& in_arrayBufferIndex,
		TArrayPointerSoundBuffer& inout_arraySoundBuffer
		);
	void CreateSoundSourceDynamicGroup(
		const TArrayInt& in_arrayBufferIndex,
		TArrayPointerSoundBuffer& inout_arraySoundBuffer
		);

	//public accessors
public:
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
	ALCcontext* mContext;
	TArrayPointerSoundSource mArraySoundSource;
	int mStaticGroupSoundSourceCount;

};

#endif 