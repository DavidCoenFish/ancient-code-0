//
//  ZombieCameraDebug.h
//  Zombie
//
//  Created by David Coen on 2011 02 12
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieCameraDebug_h_
#define _ZombieCameraDebug_h_

#include <boost/noncopyable.hpp>
#include <GVector3Float.h>

class GInput;

class ZombieCameraDebug : public boost::noncopyable
{
	//constructor
public:
	ZombieCameraDebug();
	~ZombieCameraDebug();

	//public methods
public:
	void Input(
		const GInput& in_input, 
		const GVector3Float& in_joystickDirection
		);

	//public accessors
public:
	void SetPosition(
		const GVector3Float& in_position
		);

	void SetRotation(
		const GVector3Float& in_at, 
		const GVector3Float& in_up
		);

	void GetData(
		GVector3Float& out_position,
		GVector3Float& out_at,
		GVector3Float& out_up
		);

	//private members
private:
	GVector3Float mPosition;
	GVector3Float mAt;
	GVector3Float mUp;

};

#endif //_ZombieCameraDebug_h_
