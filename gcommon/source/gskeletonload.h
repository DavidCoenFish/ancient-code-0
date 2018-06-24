//
//  GSkeletonLoad.h
//
//  Created by David Coen on 2011 01 27
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSkeletonLoad_h_
#define _GSkeletonLoad_h_

#include <boost/noncopyable.hpp>
#include <vector>

class GSkeletonBoneLoad;
class GSkeletonMorphTargetLoad;

/*
the 'image' of data to cast memory as after loading from tools output file
*/
class GSkeletonLoad : public boost::noncopyable 
{
	//constructor
public:
	GSkeletonLoad(
		const int in_boneCount = 0,
		const GSkeletonBoneLoad* const in_arraySkeletonBoneLoad = 0,
		const int in_morphTargetCount = 0,
		const GSkeletonMorphTargetLoad* const in_arraySkeletonMorphTargetLoad = 0
		);
	~GSkeletonLoad();

	//public accessors
public:
	const int GetBoneCount()const{ return mBoneCount; }
	const GSkeletonBoneLoad* const GetArraySkeletonBoneLoad()const{ return mArraySkeletonBoneLoad; }

	const int GetMorphTargetCount()const{ return mMorphTargetCount; }
	const GSkeletonMorphTargetLoad* const GetArraySkeletonMorphTargetLoad()const{ return mArraySkeletonMorphTargetLoad; }

	//private members;
private:
	const int mBoneCount;
	const GSkeletonBoneLoad* const mArraySkeletonBoneLoad;
	const int mMorphTargetCount;
	const GSkeletonMorphTargetLoad* const mArraySkeletonMorphTargetLoad;

};

#endif 