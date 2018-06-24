//
//  GAnimationManagerLoad.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationManagerLoad_h_
#define _GAnimationManagerLoad_h_

#include <boost/noncopyable.hpp>

class GAnimationStreamInfoLoad;
class GAnimationLoad;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GAnimationManagerLoad : public boost::noncopyable 
{
	//constructor
public:
	GAnimationManagerLoad(
		const unsigned int in_arrayStreamInfoCount,
		const GAnimationStreamInfoLoad* const in_arrayStreamInfo,
		const unsigned int in_arrayAnimationCount,
		const GAnimationLoad* const in_arrayAnimation
		);
	~GAnimationManagerLoad();

	//public accessors
public:
	const unsigned int GetArrayStreamInfoCount()const{ return mArrayStreamInfoCount; }
	const GAnimationStreamInfoLoad* const GetArrayStreamInfo()const{ return mArrayStreamInfo; }

	const unsigned int GetArrayAnimationCount()const{ return mArrayAnimationCount; }
	const GAnimationLoad* const GetArrayAnimation()const{ return mArrayAnimation; }

	//private members;
private:
	const unsigned int mArrayStreamInfoCount;
	const GAnimationStreamInfoLoad* const mArrayStreamInfo;
	const unsigned int mArrayAnimationCount;
	const GAnimationLoad* const mArrayAnimation;

};

#endif 