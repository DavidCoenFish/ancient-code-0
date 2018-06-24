//
//  GSceneNodeComponentSpringBoneLoad.h
//
//  Created by David Coen on 2011 01 11
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentSpringBoneLoad_h_
#define _GSceneNodeComponentSpringBoneLoad_h_

#include <boost/noncopyable.hpp>

class GSceneNodeComponentSpringBoneDataLoad;

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentSpringBoneLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentSpringBoneLoad(
		const int in_arraySpringBoneDataCount,
		const GSceneNodeComponentSpringBoneDataLoad* const in_arraySpringBoneData
		);
	~GSceneNodeComponentSpringBoneLoad();

	//public accessors
public:
	const int GetArraySpringBoneDataCount()const{ return mArraySpringBoneDataCount; }
	const GSceneNodeComponentSpringBoneDataLoad* const GetArraySpringBoneData()const{ return mArraySpringBoneData; }

	//private members;
private:
	const int mArraySpringBoneDataCount;
	const GSceneNodeComponentSpringBoneDataLoad* const mArraySpringBoneData;

};

#endif 