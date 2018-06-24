//
//  ginputbutton.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#include "ginputbutton.h"
#include "gassert.h"

//constructor
GInputButton::GInputButton()
: mTimer(0.0F)
, mDown(false)
, mEdgeUp(false)
, mEdgeDown(false)
, mTap(false)
, mRepeat(false)
{
}

GInputButton::~GInputButton()
{
	return;
}

//public methods
void GInputButton::Tick(const float in_timeDelta)
{
	mTimer += in_timeDelta;
	return;
}

//public accessors
void GInputButton::SetState(const bool in_down)
{
	mTap = false;
	mRepeat = false;

	if (mDown != in_down)
	{
		mEdgeUp = !in_down;
		mEdgeDown = in_down;
		mDown = in_down;

		if (mEdgeDown)
		{
			mTimer = 0.0F;
		}

		if (mEdgeUp && (mTimer < 0.3F))
		{
			mTap = true;
		}
		if (mDown && (0.3F < mTimer))
		{
			mRepeat = true;
			mTimer -= 0.2F;
		}
	}
	else
	{
		mEdgeUp = false;
		mEdgeDown = false;
	}

	return;
}

