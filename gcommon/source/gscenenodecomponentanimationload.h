//
//  GSceneNodeComponentAnimationLoad.h
//
//  Created by David Coen on 2011 01 05
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentAnimationLoad_h_
#define _GSceneNodeComponentAnimationLoad_h_

#include <boost/noncopyable.hpp>

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentAnimationLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentAnimationLoad(
		const char* const in_animationAssetFileName,
		const char* const in_defaultAnimationName
		);
	~GSceneNodeComponentAnimationLoad();

	//public accessors
public:
	const char* const GetAnimationAssetFileName()const{ return mAnimationAssetFileName; }
	const char* const GetDefaultAnimationName()const{ return mDefaultAnimationName; }

	//private members;
private:
	const char* const mAnimationAssetFileName;
	const char* const mDefaultAnimationName;

};

#endif 