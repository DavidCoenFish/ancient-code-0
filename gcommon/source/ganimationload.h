//
//  GAnimationLoad.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationLoad_h_
#define _GAnimationLoad_h_

#include <boost/noncopyable.hpp>

class GAnimationStreamDataLoad;
class GAnimationNoteTrackLoad;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GAnimationLoad : public boost::noncopyable 
{
	//constructor
public:
	GAnimationLoad(
		const char* const in_name,
		const float in_length,
		const int in_flag,
		const GAnimationStreamDataLoad* const in_arrayStreamDataLoad
		);
	~GAnimationLoad();

	//public accessors
public:
	const char* const GetName()const{ return mName; }
	const float GetLength()const{ return mLength; }
	const int GetFlag()const{ return mFlag; }
	const GAnimationStreamDataLoad* const GetArrayStreamDataLoad()const{ return mArrayStreamDataLoad; }

	//private members;
private:
	const char* const mName;
	const float mLength;
	const int mFlag;
	const GAnimationStreamDataLoad* const mArrayStreamDataLoad;

};

#endif 