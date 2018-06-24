//
//  GAnimationStreamInfoLoad.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GAnimationStreamInfoLoad_h_
#define _GAnimationStreamInfoLoad_h_

#include <boost/noncopyable.hpp>

class GAnimationKeyLoad;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GAnimationStreamInfoLoad : public boost::noncopyable 
{
	//constructor
public:
	GAnimationStreamInfoLoad(
		const char* const in_targetName, 
		const int in_targetType
		);
	~GAnimationStreamInfoLoad();

	//public accessors
public:
	const char* const GetTargetName()const{ return mTargetName; }
	const int GetTargetType()const{ return mTargetType; }

	//private members;
private:
	const char* const mTargetName; //bone name
	const int mTargetType; //transform_matrix, transform_pos, transform_rotation, transform_scale

};

#endif 