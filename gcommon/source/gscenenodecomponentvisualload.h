//
//  GSceneNodeComponentVisualLoad.h
//
//  Created by David Coen on 2010 12 25
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentVisualLoad_h_
#define _GSceneNodeComponentVisualLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"

class GSceneNodeLoad;

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentVisualLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentVisualLoad(
		const char* const in_meshName,
		const char* const in_materialName,
		const float in_radius,
		const char* const in_data,
		const int in_flag
		);
	~GSceneNodeComponentVisualLoad();

	//public accessors
public:
	const char* const GetMeshName()const{ return mMeshName; }
	const char* const GetMaterialName()const{ return mMaterialName; }
	const float GetRadius()const{return mRadius; }
	const char* const GetData()const{ return mData; }
	const int GetFlag()const{ return mFlag; }

	//private members;
private:
	const char* const mMeshName;
	const char* const mMaterialName;
	const float mRadius;
	const char* const mData;
	const int mFlag;

};

#endif 