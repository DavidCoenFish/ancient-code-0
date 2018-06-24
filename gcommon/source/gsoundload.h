//
//  GSoundLoad.h
//
//  Created by David Coen on 2011 03 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSoundLoad_h_
#define _GSoundLoad_h_

#include <boost/noncopyable.hpp>

/*
the 'image' of data to cast memory as after loading from tools output file
*/
struct GSoundLoad
{
public:
	const char* const GetResourceName()const{ return mResourceName; }

public:
	const char* const mResourceName; //buffer name
	//const int mConsecutiveCount; //how many of these samples can play at once
};

#endif 