//
//  GSceneNodeComponentLoad.h
//
//  Created by David Coen on 2010 12 25
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentLoad_h_
#define _GSceneNodeComponentLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"
#include "GSceneType.h"

class GSceneNodeComponentLoad;

/*
param for manual creation of node, or memory image for load from buffer
*/
class GSceneNodeComponentLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentLoad(
		const GSceneType::TComponentFlag::TFlag in_type,
		const void* const in_data
		);
	~GSceneNodeComponentLoad();

	//public accessors
public:
	const GSceneType::TComponentFlag::TFlag GetType()const{ return (GSceneType::TComponentFlag::TFlag)mType; }
	const void* const GetData()const{ return mData; }

	//private members;
private:
	const int mType;
	const void* const mData;

};

#endif 