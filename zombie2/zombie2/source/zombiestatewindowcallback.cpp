//
//  ZombieStateWindowCallback.cpp
//  Zombie
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "ZombieStateWindowCallback.h"

#include "applicationtaskgame.h"

#include <GMath.h>

//constructor
ZombieStateWindowCallback::ZombieStateWindowCallback(ZombieState& inout_parent)
: mParent(inout_parent)
{
	return;
}

ZombieStateWindowCallback::~ZombieStateWindowCallback()
{
	return;
}

//implement GApplicationWindowCallback
/*virtual*/ void ZombieStateWindowCallback::OnResize(const GVector2Float& in_size)
{
	mParent.ResizeWindow(in_size);
	return;
}

/*virtual*/ void ZombieStateWindowCallback::OnShake()
{
	mParent.ShakeWindow();
	return;
}

/*virtual*/ void ZombieStateWindowCallback::OnDismissDialog()
{
	//mParent.DismissDialogWindow();
	return;
}

/*virtual*/ void ZombieStateWindowCallback::OnInput(const GInput& in_input)
{
	mParent.InputWindow(in_input);
	return;
}

/*virtual*/ const bool ZombieStateWindowCallback::OnTick(const float in_timeDelta)
{
	const float loaclMaxTick = GMath::Minimum(0.1F, in_timeDelta);
	return mParent.TickWindow(loaclMaxTick);
}

/*virtual*/ void ZombieStateWindowCallback::OnDraw()
{
	mParent.DrawWindow();
	return;
}
