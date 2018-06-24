//
//  GAnimationKeyLoad.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationKeyLoad_h_
#define _GAnimationKeyLoad_h_

#include <boost/noncopyable.hpp>

class GAnimationStreamLoad;
class GAnimationNoteTrackLoad;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GAnimationKeyLoad : public boost::noncopyable 
{
	//constructor
public:
	GAnimationKeyLoad(
		const float in_time,
		const void* const in_data
		);
	~GAnimationKeyLoad();

	//public accessors
public:
	const float GetTime()const{ return mTime; }
	const void* const GetData()const{ return mData; }

	//private members;
private:
	const float mTime;
	const void* const mData;

};

#endif 