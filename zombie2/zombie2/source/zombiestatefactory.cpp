//
//  ZombieStateFactory.cpp
//  Zombie
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieStateFactory.h"

#include "applicationtaskgame.h"

//constructor
ZombieStateFactory::ZombieStateFactory(TPointerDictionary& inout_paramObject)
: mParamObject(inout_paramObject)
{
	return;
}

ZombieStateFactory::~ZombieStateFactory()
{
	return;
}

//implement GStateFactory
ZombieStateFactory::TPointerState ZombieStateFactory::OnCreateState()
{
	TPointerState pointerState;
	TPointerDictionary pointerParamObject = mParamObject.lock();
	pointerState.reset(new ApplicationTaskGame(pointerParamObject));

	return pointerState;
}
