//
//  GSceneNodeComponentBase.h
//
//  Created by David Coen on 10/09/30.
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#include "GSceneNodeComponentBase.h"

//constructor
GSceneNodeComponentBase::GSceneNodeComponentBase(const unsigned int in_componentFlag)
: mComponentFlag(in_componentFlag)
{
	return;
}

/*virtual*/ GSceneNodeComponentBase::~GSceneNodeComponentBase()
{
	return;
}
