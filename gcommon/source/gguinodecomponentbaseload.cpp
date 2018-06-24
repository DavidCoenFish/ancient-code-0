//
//  GGuiNodeComponentBaseLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentBaseLoad.h"

//constructor
GGuiNodeComponentBaseLoad::GGuiNodeComponentBaseLoad(
	const GGuiType::TComponentFlag::TFlag in_type,
	const void* const in_data
	)
	: mType(in_type)
	, mData(in_data)
{
	return;
}

GGuiNodeComponentBaseLoad::~GGuiNodeComponentBaseLoad()
{
	return;
}
