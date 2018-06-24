//
//  GSoundSource.h
//
//  Created by David Coen on 2011 05 02
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSoundSource_h_
#define _GSoundSource_h_

#include <boost/noncopyable.hpp>
#include <boost/scoped_ptr.hpp>
#include <string>

#ifdef IPHONE
	class GIPhoneSound;
	typedef GIPhoneSound GSoundSourceImplementation;

#elif WIN32
	class GWin32Sound;
	typedef GWin32Sound GSoundSourceImplementation;
#endif

class GVector3Float;

class GSoundSource : public boost::noncopyable
{
	//typedef
private:

	//constructor
public:
	GSoundSource(
		const unsigned int in_bufferId
		);
	~GSoundSource();

	//public methods
public:
	void Play();
	void Pause();
	void Stop();

	//public accessors
public:
	void SetVolume(const float in_volume);
	void SetPan(const float in_pan);
	const bool GetIsPlaying()const;
	void SetPosition(const GVector3Float& in_position);
	void SetOmni(const bool in_omni);
	void SetLoop(const bool in_loop);

	//private members;
private:
	unsigned int mSourceId;

};

#endif 