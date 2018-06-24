//
//  GSceneNodeLoad.h
//
//  Created by David Coen on 2010 12 25
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeLoad_h_
#define _GSceneNodeLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"

class GSceneNodeComponentLoad;

/*
param for manual creation of node, or memory image for load from buffer
*/
class GSceneNodeLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeLoad(
		const char* const in_name,
		const GMatrix16Float* const in_transformParentRelative,
		const int in_componentCount,
		const GSceneNodeComponentLoad* const in_arrayComponentLoad,
		const int in_childCount,
		const GSceneNodeLoad* const in_arrayChild
		);
	~GSceneNodeLoad();

	//public accessors
public:
	const char* const GetName()const{ return mName; }
	const GMatrix16Float& GetTransformParentRelative()const{ return *mTransformParentRelative; }

	const int GetComponentCount()const{ return mComponentCount; }
	const GSceneNodeComponentLoad& GetComponentLoad(const int in_index)const;

	const int GetChildCount()const{ return mChildCount; }
	const GSceneNodeLoad& GetChild(const int in_index)const;

	//private members;
private:
	const char* const mName;
	const GMatrix16Float* const mTransformParentRelative;

	const int mComponentCount;
	const GSceneNodeComponentLoad* const mArrayComponentLoad;

	const int mChildCount;
	const GSceneNodeLoad* const mArrayChild;

};

#endif 