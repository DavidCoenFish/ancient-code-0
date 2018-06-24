//
//  GSceneNodeComponentNodeAttachLoad.h
//
//  Created by David Coen on 2011 01 04
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GSceneNodeComponentVisualAttachmentLoad_h_
#define _GSceneNodeComponentVisualAttachmentLoad_h_

#include <boost/noncopyable.hpp>
#include "GMatrix16Float.h"
#include "GSceneNodeLoad.h"

class GSceneNodeComponentLoad;

/*
param for manual creation of component, or memory image for load from buffer
*/
class GSceneNodeComponentNodeAttachLoad : public boost::noncopyable
{
	//constructor
public:
	GSceneNodeComponentNodeAttachLoad(
		const char* const in_boneName,
		const char* const in_name,
		const GMatrix16Float& in_transformParentRelative,
		const int in_componentCount,
		const GSceneNodeComponentLoad* const in_arrayComponentLoad,
		const int in_childCount,
		const GSceneNodeLoad* const in_arrayChild
		);
	~GSceneNodeComponentNodeAttachLoad();

	//public accessors
public:
	const char* const GetBoneName()const{ return mBoneName; }
	const GMatrix16Float& GetTransformParentRelative()const;
	const GSceneNodeLoad& GetSceneNodeLoad()const{ return mSceneNodeLoad; }

	//private members;
private:
	const char* const mBoneName;
	GSceneNodeLoad mSceneNodeLoad;

};

#endif 