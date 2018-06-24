//
//  PlayWithGoldfishStateWindowCallback.cpp
//  playwithgoldfish
//
//  Created by David Coen on 2010 12 30
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#include "PlayWithGoldfishStateWindowCallback.h"

#include "PlayWithGoldfishState.h"
#include "GVector2Int.h"

//constructor
PlayWithGoldfishStateWindowCallback::PlayWithGoldfishStateWindowCallback(PlayWithGoldfishState& inout_parent)
: mParent(inout_parent)
{
	return;
}

PlayWithGoldfishStateWindowCallback::~PlayWithGoldfishStateWindowCallback()
{
	return;
}

//implement GApplicationWindowCallback
/*virtual*/ void PlayWithGoldfishStateWindowCallback::OnResize(const GVector2Int& in_size)
{
	mParent.ResizeWindow(in_size);
	return;
}

/*virtual*/ void PlayWithGoldfishStateWindowCallback::OnShake()
{
	mParent.ShakeWindow();
	return;
}

/*virtual*/ void PlayWithGoldfishStateWindowCallback::OnDismissDialog()
{
	mParent.DismissDialogWindow();
	return;
}

/*virtual*/ void PlayWithGoldfishStateWindowCallback::OnInput(const GInput& in_input)
{
	mParent.InputWindow(in_input);
	return;
}

/*virtual*/ const bool PlayWithGoldfishStateWindowCallback::OnTick(const float in_timeDelta)
{
	return mParent.TickWindow(in_timeDelta);
}

/*virtual*/ void PlayWithGoldfishStateWindowCallback::OnDraw()
{
	mParent.DrawWindow();
	return;
}
