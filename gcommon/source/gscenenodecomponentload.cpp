//
//  GSceneNodeComponentLoad.cpp
//
//  Created by David Coen on 2010 12 25
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentLoad.h"

//constructor
GSceneNodeComponentLoad::GSceneNodeComponentLoad(
	const GSceneType::TComponentFlag::TFlag in_type,
	const void* const in_data
	)
	: mType(in_type)
	, mData(in_data)
{
	return;
}

GSceneNodeComponentLoad::~GSceneNodeComponentLoad()
{
	return;
}
