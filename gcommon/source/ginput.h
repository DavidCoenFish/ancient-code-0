//
//  ginput.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//

#ifndef _GInput_h_
#define _GInput_h_

#include "gcommon.h"
#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <vector>

class GInputJoystick;
class GInputTouch;
class GInputButton;
class GVector2Float;

class GInput : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GInputJoystick> TPointerInputJoystick;
	typedef std::vector<TPointerInputJoystick> TArrayPointerInputJoystick;

	typedef boost::shared_ptr<GInputTouch> TPointerInputTouch;
	typedef std::vector<TPointerInputTouch> TArrayPointerInputTouch;

	typedef boost::shared_ptr<GInputButton> TPointerInputButton;
	typedef std::vector<TPointerInputButton> TArrayPointerInputButton;

	//constructor
public:
	GInput();
	~GInput();

	//public methods
public:
	void Tick(const float in_timeDelta);

	void AddTouch(
		const unsigned int in_touchId, 
		const GVector2Float& in_touchLocation,
		const GVector2Float& in_touchDelta,
		const bool in_touchBegin,
		const bool in_touchEnd,
		const bool in_touchCancel
		);
		
	//public accessors
public:
	void SetJoystickCount(const int in_joystickCount);
	const int GetJoystickCount()const;

	void SetTouchCount(const int in_touchCount);
	const int GetTouchCount()const;

	void SetButtonCount(const int in_buttonCount);
	const int GetButtonCount()const;

	const GInputJoystick& GetJoystick(const int in_index)const;
	GInputJoystick& GetJoystick(const int in_index);

	const GInputTouch& GetTouch(const int in_index)const;
	GInputTouch& GetTouch(const int in_index);

	const GInputButton& GetButton(const int in_index)const;
	GInputButton& GetButton(const int in_index);

	//private members;
private:
	TArrayPointerInputJoystick mArrayJoystick;
	TArrayPointerInputTouch mArrayTouch;
	TArrayPointerInputButton mArrayButton;

};

#endif 