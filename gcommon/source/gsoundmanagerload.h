//
//  GSoundManagerLoad.h
//
//  Created by David Coen on 2011 03 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSoundManagerLoad_h_
#define _GSoundManagerLoad_h_

#include <boost/noncopyable.hpp>

struct GSoundLoad;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GSoundManagerLoad : public boost::noncopyable 
{
	//constructor
public:
	GSoundManagerLoad(
		const unsigned int in_arraySoundLoadCount,
		const GSoundLoad* const in_arraySoundLoad
		);
	~GSoundManagerLoad();

	//public accessors
public:
	const unsigned int GetArraySoundLoadCount()const{ return mArraySoundLoadCount; }
	const GSoundLoad* const GetArraySoundLoad()const{ return mArraySoundLoad; }

	//private members;
private:
	const unsigned int mArraySoundLoadCount;
	const GSoundLoad* const mArraySoundLoad;

};

#endif 