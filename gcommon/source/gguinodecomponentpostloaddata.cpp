//
//  GGuiNodeComponentPostLoadData.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiNodeComponentPostLoadData.h"

//constructor
GGuiNodeComponentPostLoadData::GGuiNodeComponentPostLoadData(
	GGuiNode* const in_guiNode,
	GGuiNodeComponentBase* const in_guiNodeComponentBase,
	void* const in_data	
	)
	: mGuiNode(in_guiNode)
	, mGuiNodeComponentBase(in_guiNodeComponentBase)
	, mData(in_data)
{
	return;
}

GGuiNodeComponentPostLoadData::~GGuiNodeComponentPostLoadData()
{
	return;
}
