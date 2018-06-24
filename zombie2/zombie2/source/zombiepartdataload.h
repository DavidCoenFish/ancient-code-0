//
//  ZombiePartDataLoad.h
//  Zombie
//
//  Created by David Coen on 2011 02 28
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombiePartDataLoad_h_
#define _ZombiePartDataLoad_h_

#include <boost/noncopyable.hpp>

struct ZombiePartDataLoad// : public boost::noncopyable
{
//	//constructor
//public:
//	ZombiePartDataLoad(
//		const char* const in_visualData,
//		const float in_value
//		);
//	~ZombiePartDataLoad();

	//public accessors
public:
	const char* const GetVisualData()const{ return mVisualData; }
	const float GetValue()const{ return mValue; }

	//private members
public:
	const char* const mVisualData; // soft bind to data in visual/ collision
	const float mValue;
	//const bool mPartSkinned; //non skinned parts need to be detached differently

};

#endif //_ZombiePartDataLoad_h_
