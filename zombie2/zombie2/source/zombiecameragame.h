//
//  ZombieCameraGame.h
//  Zombie
//
//  Created by David Coen on 2011 02 12
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieCameraGame_h_
#define _ZombieCameraGame_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <GComponentLerp.h>
#include <GVector3Float.h>

class GInput;
class GComponentLerpFloat;
class GComponentLerpVector3Float;

class ZombieCameraGame : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GComponentLerpVector3Float> TPointerComponentLerpVector3Float;
	typedef boost::shared_ptr<GComponentLerpFloat> TPointerComponentLerpFloat;

	//constructor
public:
	ZombieCameraGame();
	~ZombieCameraGame();

	//public methods
public:
	void Tick(const float in_timeDelta);
	void Input(
		const GInput& in_input, 
		const GVector3Float& in_joystickDirection
		);
	//public accessors
public:
	void SetPosition(
		const GVector3Float& in_position, 
		const float in_lerpDuration = 0.0F,
		const GComponentLerp::TType::TEnum in_lerpType = GComponentLerp::TType::TLinear
		);

	void SetRotation(
		const GVector3Float& in_at, 
		const GVector3Float& in_up, 
		const float in_lerpDuration = 0.0F,
		const GComponentLerp::TType::TEnum in_lerpType = GComponentLerp::TType::TLinear
		);

	void GetData(
		GVector3Float& out_position,
		GVector3Float& out_at,
		GVector3Float& out_up
		);

	//private members
private:
	TPointerComponentLerpVector3Float mLerpPosition;
	TPointerComponentLerpFloat mLerpFacing;

	float mRize; //longitude, deg updown from horizon, from accelerometor
	float mRoll; //make the iphone accelerometor define what is up and down for the screen

};

#endif //_ZombieCameraGame_h_
