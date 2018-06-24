//
//  GSceneLoad.h
//
//  Created by David Coen on 2010 12 25
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneLoad_h_
#define _GSceneLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"

class GSceneNodeLoad;

/*
param for manual creation of scene, or memory image for load from buffer
*/
class GSceneLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneLoad(
		const int in_sceneNodeLoadCount,
		const GSceneNodeLoad* const in_arraySceneNodeLoad
		);
	~GSceneLoad();

	//public accessors
public:
	const int GetSceneNodeLoadCount()const{ return mSceneNodeLoadCount; }
	const GSceneNodeLoad& GetSceneNodeLoad(const int in_index)const;

	//private members;
private:
	const int mSceneNodeLoadCount;
	const GSceneNodeLoad* const mArraySceneNodeLoad;

};

#endif 