//
//  GGuiLoad.h
//
//  Created by David Coen on 2011 04 12
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GGuiLoad.h"

//constructor
GGuiLoad::GGuiLoad(
	const unsigned int in_arrayGuiNodeCount,
	const GGuiNodeLoad* const in_arrayGuiNode		
	)
	: mArrayGuiNodeCount(in_arrayGuiNodeCount)
	, mArrayGuiNode(in_arrayGuiNode)
{
	return;
}

GGuiLoad::~GGuiLoad()
{
	return;
}
