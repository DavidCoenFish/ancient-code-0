//
//  GSceneNodeComponentFadeLoad.h
//
//  Created by David Coen on 2011 02 08
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentFadeLoad_h_
#define _GSceneNodeComponentFadeLoad_h_

#include <boost/noncopyable.hpp>

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentFadeLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentFadeLoad(
		const float in_duration
		);
	~GSceneNodeComponentFadeLoad();

	//public accessors
public:
	const float GetDuration()const{ return mDuration; }

	//private members;
private:
	const float mDuration;

};

#endif 