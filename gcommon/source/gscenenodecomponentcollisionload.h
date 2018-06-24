//
//  GSceneNodeComponentCollisionLoad.h
//
//  Created by David Coen on 2011 02 21
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentCollisionLoad_h_
#define _GSceneNodeComponentCollisionLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"

class GSceneNodeLoad;
class GSceneNodeComponentCollisionDataLoad;
/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentCollisionLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentCollisionLoad(
		const int in_materialId,
		const char* const in_data,
		const int in_flag,
		const int in_collisionDataCount,
		const GSceneNodeComponentCollisionDataLoad* const in_arrayCollisionData
		);
	~GSceneNodeComponentCollisionLoad();

	//public accessors
public:
	const int GetMaterialId()const{ return mMaterialId; }
	const char* const GetData()const{ return mData; }
	const bool GetFlagActive()const{ return 0 != mFlag; }
	const int GetCollisionDataCount()const{ return mCollisionDataCount; }
	const GSceneNodeComponentCollisionDataLoad* const GetArrayCollisionData()const{ return mArrayCollisionData; }

	//private members;
private:
	const int mMaterialId;
	const char* const mData;
	const int mFlag;
	const int mCollisionDataCount;
	const GSceneNodeComponentCollisionDataLoad* const mArrayCollisionData;

};

#endif 