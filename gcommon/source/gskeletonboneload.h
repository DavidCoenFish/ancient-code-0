//
//  GSkeletonBoneLoad.h
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSkeletonBoneLoad_h_
#define _GSkeletonBoneLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GSkeletonBoneLoad : public boost::noncopyable 
{
	//constructor
public:
	GSkeletonBoneLoad(
		const char* const in_boneName = "",
		const int in_parentIndex = -1,
		const GMatrix16Float& in_parentRelativeMatrix = GMatrix16Float::sIdentity
		);
	~GSkeletonBoneLoad();

	//public accessors
public:
	const char* const GetBoneName()const{ return mBoneName; }
	const int GetParentIndex()const{ return mParentIndex; }
	const GMatrix16Float& GetParentRelativeMatrix()const{ return mParentRelativeMatrix; }

	//private members;
private:
	const char* const mBoneName;
	const int mParentIndex;
	const GMatrix16Float mParentRelativeMatrix;

};

#endif 