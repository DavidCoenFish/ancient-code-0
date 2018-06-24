//
//  GGuiNodeLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GGuiNodeLoad_h_
#define _GGuiNodeLoad_h_

#include <boost/noncopyable.hpp>
#include "GGuiCoordinate.h"

class GGuiNodeComponentBaseLoad;

class GGuiNodeLoad : public boost::noncopyable
{
	//constructor
public:
	GGuiNodeLoad(
		const char* const in_name,
		const GU32 in_flag,
		const GGuiCoordinate& in_topLeft,
		const GGuiCoordinate& in_size,
		const GU32 in_arrayComponentCount,
		const GGuiNodeComponentBaseLoad* const in_arrayComponent,
		const GU32 in_arrayGuiNodeCount,
		const GGuiNodeLoad* const in_arrayGuiNode
		);
	~GGuiNodeLoad();

	//public accessors
public:
	const char* const GetName()const{ return mName; }
	const GU32 GetFlag()const{ return mFlag; }
	const GGuiCoordinate& GetTopLeft()const{ return mTopLeft; }
	const GGuiCoordinate& GetSize()const{ return mSize; }
	const GU32 GetArrayComponentCount()const{ return mArrayComponentCount; }
	const GGuiNodeComponentBaseLoad* const GetArrayComponent()const{ return mArrayComponent; }	

	const GU32 GetArrayGuiNodeCount()const{ return mArrayGuiNodeCount; }
	const GGuiNodeLoad* const GetArrayGuiNode()const{ return mArrayGuiNode; }	

	//private members;
private:
	const char* const mName;
	const GU32 mFlag;
	const GGuiCoordinate mTopLeft;
	const GGuiCoordinate mSize;

	const GU32 mArrayComponentCount;
	const GGuiNodeComponentBaseLoad* const mArrayComponent;

	const GU32 mArrayGuiNodeCount;
	const GGuiNodeLoad* const mArrayGuiNode;

};

#endif 