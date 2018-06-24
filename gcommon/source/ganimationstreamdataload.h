//
//  GAnimationStreamDataLoad.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationStreamDataLoad_h_
#define _GAnimationStreamDataLoad_h_

#include <boost/noncopyable.hpp>

class GAnimationKeyLoad;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GAnimationStreamDataLoad : public boost::noncopyable 
{
	//constructor
public:
	GAnimationStreamDataLoad(
		const unsigned int in_arrayKeyCount,
		const GAnimationKeyLoad* const in_arrayKeyLoad
		);
	~GAnimationStreamDataLoad();

	//public accessors
public:
	const unsigned int GetArrayKeyCount()const{ return mArrayKeyCount; }
	const GAnimationKeyLoad* const GetArrayKeyLoad()const{ return mArrayKeyLoad; }

	//private members;
private:
	const unsigned int mArrayKeyCount;
	const GAnimationKeyLoad* const mArrayKeyLoad;

};

#endif 