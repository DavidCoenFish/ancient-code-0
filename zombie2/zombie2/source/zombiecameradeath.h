//
//  ZombieCameraDeath.h
//  Zombie
//
//  Created by David Coen on 2011 02 25
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieCameraDeath_h_
#define _ZombieCameraDeath_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <GComponentLerp.h>
#include <GVector3Float.h>

class GInput;
class GComponentLerpVector3Float;

/*
the player has died, this camera drops camera to the ground and mucks around with the pitch
*/
class ZombieCameraDeath : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GComponentLerpVector3Float> TPointerComponentLerpVector3Float;

	//constructor
public:
	ZombieCameraDeath();
	~ZombieCameraDeath();

	//public methods
public:
	void Tick(const float in_timeDelta);
	void Input(
		const GInput& in_input, 
		const GVector3Float& in_joystickDirection
		);
	//public accessors
public:
	void Activate(
		const GVector3Float& in_position,
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
	TPointerComponentLerpVector3Float mLerpPosition;
	TPointerComponentLerpVector3Float mLerpAt;
	TPointerComponentLerpVector3Float mLerpUp;
	float mRoll;

};

#endif //_ZombieCameraDeath_h_
