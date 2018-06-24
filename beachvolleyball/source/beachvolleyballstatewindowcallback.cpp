//
//  BeachVolleyBallStateWindowCallback.cpp
//  BeachVolleyBall
//
//  Created by David Coen on 2011 04 03
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "BeachVolleyBallStateWindowCallback.h"

#include "BeachVolleyBallState.h"
#include <GVector2Int.h>
#include <GMath.h>

//constructor
BeachVolleyBallStateWindowCallback::BeachVolleyBallStateWindowCallback(BeachVolleyBallState& inout_parent)
: mParent(inout_parent)
{
	return;
}

BeachVolleyBallStateWindowCallback::~BeachVolleyBallStateWindowCallback()
{
	return;
}

//implement GApplicationWindowCallback
/*virtual*/ void BeachVolleyBallStateWindowCallback::OnResize(const GVector2Int& in_size)
{
	mParent.ResizeWindow(in_size);
	return;
}

/*virtual*/ void BeachVolleyBallStateWindowCallback::OnShake()
{
	mParent.ShakeWindow();
	return;
}

/*virtual*/ void BeachVolleyBallStateWindowCallback::OnDismissDialog()
{
	//mParent.DismissDialogWindow();
	return;
}

/*virtual*/ void BeachVolleyBallStateWindowCallback::OnInput(const GInput& in_input)
{
	mParent.InputWindow(in_input);
	return;
}

/*virtual*/ const bool BeachVolleyBallStateWindowCallback::OnTick(const float in_timeDelta)
{
	const float loaclMaxTick = GMath::Minimum(0.1F, in_timeDelta);
	return mParent.TickWindow(loaclMaxTick);
}

/*virtual*/ void BeachVolleyBallStateWindowCallback::OnDraw()
{
	mParent.DrawWindow();
	return;
}
