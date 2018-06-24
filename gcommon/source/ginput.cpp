//
//  ginput.cpp
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#include "ginput.h"

#include "gassert.h"
#include "ginputbutton.h"
#include "ginputjoystick.h"
#include "ginputtouch.h"

//constructor
GInput::GInput()
: mArrayJoystick()
, mArrayTouch()
, mArrayButton()
{
	return;
}

GInput::~GInput()
{
	return;
}

//public methods
void GInput::Tick(const float in_timeDelta)
{
	for (TArrayPointerInputButton::iterator iterator = mArrayButton.begin(); iterator != mArrayButton.end(); ++iterator)
	{
		if (*iterator)
		{
			(*iterator)->Tick(in_timeDelta);
		}
	}

	TArrayPointerInputTouch copyArrayTouch(mArrayTouch);
	mArrayTouch.clear();
	
	for (TArrayPointerInputTouch::iterator iterator = copyArrayTouch.begin(); iterator != copyArrayTouch.end(); ++iterator)
	{
		GInputTouch& touch = *(*iterator);
		if (touch.GetTouchEnd() ||
			touch.GetTouchCancel())
		{	
			continue;
		}
		touch.SetTouchBegin(false);
		touch.ClearDelta();
		mArrayTouch.push_back(*iterator);
	}

	return;
}

void GInput::AddTouch(
	const unsigned int in_touchId, 
	const GVector2Float& in_touchLocation,
	const GVector2Float& in_touchDelta,
	const bool in_touchBegin,
	const bool in_touchEnd,
	const bool in_touchCancel
	)
{
	for (TArrayPointerInputTouch::iterator iterator =  mArrayTouch.begin(); iterator != mArrayTouch.end(); ++iterator)
	{
		if ((*iterator) && ((*iterator)->GetTouchId() == in_touchId))
		{
			(*iterator)->UpdateLocation(in_touchLocation, in_touchBegin);
			if (in_touchEnd)
			{
				(*iterator)->SetTouchEnd(true);			
			}
			else if (in_touchCancel)
			{
				(*iterator)->SetTouchCancel(true);			
			}
			return;
		}
	}

	//if (in_touchBegin)
	{
		TPointerInputTouch pointer;
		pointer.reset(new GInputTouch(in_touchId, in_touchLocation, in_touchDelta, in_touchBegin, in_touchEnd, in_touchCancel));
		mArrayTouch.push_back(pointer);
	}

	return;
}

//public accessors
void GInput::SetJoystickCount(const int in_joystickCount)
{
	mArrayJoystick.resize(in_joystickCount);
	for (TArrayPointerInputJoystick::iterator iterator =  mArrayJoystick.begin(); iterator != mArrayJoystick.end(); ++iterator)
	{
		if (!(*iterator))
		{
			iterator->reset(new GInputJoystick());
		}
	}

	return;
}

const int GInput::GetJoystickCount()const
{
	return mArrayJoystick.size();
}


void GInput::SetTouchCount(const int in_touchCount)
{
	mArrayTouch.resize(in_touchCount);
	for (TArrayPointerInputTouch::iterator iterator = mArrayTouch.begin(); iterator != mArrayTouch.end(); ++iterator)
	{
		if (!(*iterator))
		{
			iterator->reset(new GInputTouch());
		}
	}

	return;
}

const int GInput::GetTouchCount()const
{
	return mArrayTouch.size();
}

void GInput::SetButtonCount(const int in_buttonCount)
{
	mArrayButton.resize(in_buttonCount);
	for (TArrayPointerInputButton::iterator iterator = mArrayButton.begin(); iterator != mArrayButton.end(); ++iterator)
	{
		if (!(*iterator))
		{
			iterator->reset(new GInputButton());
		}
	}

	return;
}

const int GInput::GetButtonCount()const
{
	return mArrayButton.size();
}

const GInputJoystick& GInput::GetJoystick(const int in_index)const
{
	GASSERT(((0 <= in_index) && (in_index < GetJoystickCount())), "invalid param");
	return *mArrayJoystick[in_index];
}

GInputJoystick& GInput::GetJoystick(const int in_index)
{
	GASSERT(((0 <= in_index) && (in_index < GetJoystickCount())), "invalid param");
	return *mArrayJoystick[in_index];
}

const GInputTouch& GInput::GetTouch(const int in_index)const
{
	GASSERT(((0 <= in_index) && (in_index < GetTouchCount())), "invalid param");
	return *mArrayTouch[in_index];
}

GInputTouch& GInput::GetTouch(const int in_index)
{
	GASSERT(((0 <= in_index) && (in_index < GetTouchCount())), "invalid param");
	return *mArrayTouch[in_index];
}

const GInputButton& GInput::GetButton(const int in_index)const
{
	GASSERT(((0 <= in_index) && (in_index < GetButtonCount())), "invalid param");
	return *mArrayButton[in_index];
}

GInputButton& GInput::GetButton(const int in_index)
{
	GASSERT(((0 <= in_index) && (in_index < GetButtonCount())), "invalid param");
	return *mArrayButton[in_index];
}

