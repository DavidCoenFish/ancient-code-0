//
//  PlayWithGoldfishStateFactory.cpp
//  playwithgoldfish
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "PlayWithGoldfishStateFactory.h"

#include "PlayWithGoldfishState.h"

//constructor
PlayWithGoldfishStateFactory::PlayWithGoldfishStateFactory(TPointerDictionary& inout_paramObject)
: mParamObject(inout_paramObject)
{
	return;
}

PlayWithGoldfishStateFactory::~PlayWithGoldfishStateFactory()
{
	return;
}

//implement GStateFactory
PlayWithGoldfishStateFactory::TPointerState PlayWithGoldfishStateFactory::OnCreateState()
{
	TPointerState pointerState;
	TPointerDictionary pointerParamObject = mParamObject.lock();
	pointerState.reset(new PlayWithGoldfishState(pointerParamObject));

	return pointerState;
}
