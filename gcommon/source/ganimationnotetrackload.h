//
//  GAnimationNoteTrackLoad.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationNoteTrackLoad_h_
#define _GAnimationNoteTrackLoad_h_

#include <boost/noncopyable.hpp>

class GAnimationStreamLoad;
class GAnimationNoteTrackLoad;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GAnimationNoteTrackLoad : public boost::noncopyable 
{
	//constructor
public:
	GAnimationNoteTrackLoad(
		const float in_time,
		const char* const in_note
		);
	~GAnimationNoteTrackLoad();

	//public accessors
public:
	const float GetTime()const{ return mTime; }
	const char* const GetNote()const{ return mNote; }

	//private members;
private:
	const float mTime;
	const char* const mNote;

};

#endif 