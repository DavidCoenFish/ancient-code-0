//
//  GSkeletonMorphTargetLoad.h
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSkeletonMorphTargetLoad_h_
#define _GSkeletonMorphTargetLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GSkeletonMorphTargetLoad : public boost::noncopyable 
{
	//constructor
public:
	GSkeletonMorphTargetLoad(
		const char* const in_morphTargetName = ""
		);
	~GSkeletonMorphTargetLoad();

	//public accessors
public:
	const char* const GetMorphTargetName()const{ return mMorphTargetName; }

	//private members;
private:
	const char* const mMorphTargetName;

};

#endif 