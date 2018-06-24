//
//  ginputjoystick.h
//  GCommon
//
//  Created by David Coen on 2011 06 01
//  Copyright Pleasure seeking morons 2011. All rights reserved.
//


#ifndef _GInputJoystick_h_
#define _GInputJoystick_h_

#include "gcommon.h"
#include "gvector3float.h"
#include <boost/noncopyable.hpp>

class GInputJoystick : public boost::noncopyable
{
	//constructor
public:
	GInputJoystick();
	~GInputJoystick();

	//public accessors
public:
	const GVector3Float& GetDirection()const{ return mDirection; }
	GVector3Float& GetDirection(){ return mDirection; }

	//private members;
private:
	GVector3Float mDirection;

};

#endif 