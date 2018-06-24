//
//  ZombieCameraCinematic.h
//  Zombie
//
//  Created by David Coen on 2011 02 12
//  Copyright Pleasure seeking morons 2010. All rights reserved.
//

#ifndef _ZombieCameraCinematic_h_
#define _ZombieCameraCinematic_h_

#include <boost/noncopyable.hpp>
#include <boost/shared_ptr.hpp>
#include <GComponentLerp.h>
#include <GVector3Float.h>

class GInput;
class GComponentLerpVector3Float;

class ZombieCameraCinematic : public boost::noncopyable
{
	//typedef
private:
	typedef boost::shared_ptr<GComponentLerpVector3Float> TPointerComponentLerpVector3Float;

	//constructor
public:
	ZombieCameraCinematic();
	~ZombieCameraCinematic();

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
		GVector3Float& out_up,
		const bool in_useRoll
		);

	//private members
private:
	TPointerComponentLerpVector3Float mLerpPosition;
	TPointerComponentLerpVector3Float mLerpAt;
	TPointerComponentLerpVector3Float mLerpUp;
	float mRoll;

};

#endif //_ZombieCameraCinematic_h_
