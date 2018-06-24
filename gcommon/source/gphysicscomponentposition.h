//
//  GPhysicsComponentPosition.h
//
//  Created by David Coen on 2010 10 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GPhysicsComponentPosition_h_
#define _GPhysicsComponentPosition_h_

#include "GVector3Float.h"

/**/
class GPhysicsComponentPosition
{
	//static methods
public:
	static const GVector3Float CalculateGetVelocity(
		const GVector3Float& in_oldPosition,
		const GVector3Float& in_newPosition,
		const float in_timeDelta
		);

	//constructors
public:
	GPhysicsComponentPosition(
		const float in_massInvert = 0.0F,
		const GVector3Float& in_position = GVector3Float::sZero,
		const GVector3Float& in_velocity = GVector3Float::sZero
		);
	GPhysicsComponentPosition(const GPhysicsComponentPosition& in_src);
	~GPhysicsComponentPosition();
	
	//operators
public:
	const GPhysicsComponentPosition& operator=(const GPhysicsComponentPosition& in_rhs);
	
	//public methods
public:
	void ClearForceSum(); //clear force sum
	void AddAcceleration(const GVector3Float& in_acceleration);
	void MinusAcceleration(const GVector3Float& in_acceleration);
	void AddForce(const GVector3Float& in_force); //force = mass * acceleration
	void Intergrate(const float in_timeDelta);
	void Intergrate(const float in_timeDelta, const float in_velocityDampen);

	//set new position and new velocity .ie after collision
	void ManualSetPosition(
		const GVector3Float& in_newPosition, 
		const GVector3Float& in_newVelocity
		);

	//public accessors
public:
	const GVector3Float& GetPosition()const{ return mPosition; }
	const GVector3Float& GetVelocity()const{ return mVelocity; }

	//private members
private:
	float mInvertMass;  //(1.0F / mass)

	GVector3Float mPosition;
	GVector3Float mVelocity;

	//valid after a begin
	//GVector3Float mPositionOld; if you want to use for collision segments, create a new component class for old position and time segment, it doesn't live here though
	GVector3Float mForceSum;	

};

/**/
#endif //_GPhysicsComponentPosition_h_
