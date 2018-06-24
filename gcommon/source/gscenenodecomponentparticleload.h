//
//  GSceneNodeComponentParticleLoad.h
//
//  Created by David Coen on 2011 02 18
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentParticleLoad_h_
#define _GSceneNodeComponentParticleLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"

class GSceneNodeLoad;

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentParticleLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentParticleLoad(
		const char* const in_data,
		const char* const in_materialName,
		const float in_radius,
		const int in_defaultEmittor,
		const float in_defaultEmittorDuration,
		const int in_maxParticleCount,
		const int in_maxLineCount,
		const int in_maxEmitorCount
		);
	~GSceneNodeComponentParticleLoad();

	//public accessors
public:
	const char* const GetData()const{ return mData; }
	const char* const GetMaterialName()const{ return mMaterialName; }
	const float GetRadius()const{return mRadius; }
	const int GetDefaultEmittor()const{ return mDefaultEmittor; }
	const float GetDefaultEmittorDuration()const{ return mDefaultEmittorDuration; }
	const int GetMaxParticleCount()const{ return mMaxParticleCount; }
	const int GetMaxLineCount()const{ return mMaxLineCount; }
	const int GetMaxEmittorCount()const{ return mMaxEmittorCount; }

	//private members;
private:
	const char* const mData;
	const char* const mMaterialName;
	const float mRadius;
	const int mDefaultEmittor;
	const float mDefaultEmittorDuration;
	const int mMaxParticleCount;
	const int mMaxLineCount;
	const int mMaxEmittorCount;

};

#endif 