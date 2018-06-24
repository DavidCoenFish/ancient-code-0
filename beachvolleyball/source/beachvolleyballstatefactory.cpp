//
//  BeachVolleyBallStateFactory.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallStateFactory.h"

#include "BeachVolleyBallState.h"

//constructor
BeachVolleyBallStateFactory::BeachVolleyBallStateFactory(TPointerDictionary& inout_paramObject)
: mParamObject(inout_paramObject)
{
	return;
}

BeachVolleyBallStateFactory::~BeachVolleyBallStateFactory()
{
	return;
}

//implement GStateFactory
BeachVolleyBallStateFactory::TPointerState BeachVolleyBallStateFactory::OnCreateState()
{
	TPointerState pointerState;
	TPointerDictionary pointerParamObject = mParamObject.lock();
	pointerState.reset(new BeachVolleyBallState(pointerParamObject));

	return pointerState;
}
