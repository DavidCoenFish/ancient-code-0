//
//  GGuiNodeLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeLoad.h"

//constructor
GGuiNodeLoad::GGuiNodeLoad(
	const char* const in_name,
	const GU32 in_flag,
	const GGuiCoordinate& in_topLeft,
	const GGuiCoordinate& in_size,
	const GU32 in_arrayComponentCount,
	const GGuiNodeComponentBaseLoad* const in_arrayComponent,
	const GU32 in_arrayGuiNodeCount,
	const GGuiNodeLoad* const in_arrayGuiNode
	)
	: mName(in_name)
	, mFlag(in_flag)
	, mTopLeft(in_topLeft)
	, mSize(in_size)
	, mArrayComponentCount(in_arrayComponentCount)
	, mArrayComponent(in_arrayComponent)
	, mArrayGuiNodeCount(in_arrayGuiNodeCount)
	, mArrayGuiNode(in_arrayGuiNode)
{
	return;
}

GGuiNodeLoad::~GGuiNodeLoad()
{
	return;
}
