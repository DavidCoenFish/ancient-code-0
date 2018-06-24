//
//  GPhysicsComponentSpring.h
//
//  Created by David Coen on 2010 10 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GPhysicsComponentSpring_h_
#define _GPhysicsComponentSpring_h_

class GVector3Float;

/**/
class GPhysicsComponentSpring
{
	//constructors
public:
	GPhysicsComponentSpring(
		const float in_springConstant = 0.0F,
		const float in_springInnerFriction = 0.0F
		);
	GPhysicsComponentSpring(const GPhysicsComponentSpring& in_src);
	~GPhysicsComponentSpring();
	
	//operators
public:
	const GPhysicsComponentSpring& operator=(const GPhysicsComponentSpring& in_rhs);
	
	//public methods
public:
	const float CalculateSpringForce(
		const float in_target, //the distance we want to be from the spring origin
		const float in_currentValue
		)const;
	const float CalculateSpringFriction(
		const float in_velocityLhs,
		const float in_velocityRhs
		)const;

	const GVector3Float CalculateSpringForce(
		const GVector3Float& in_origin,
		const float in_targetDistance, //the distance we want to be from the spring origin
		const GVector3Float& in_currentValue
		)const;
	const GVector3Float CalculateSpringFriction(
		const GVector3Float& in_velocityLhs,
		const GVector3Float& in_velocityRhs
		)const;

	//public accessor
public:
	//const float GetSpringDampen()const{ return mSpringDampen; }

	//private members
private:
	float mSpringConstant;

	//inner friction of the spring, kind of like dampening, a force to resist spring movement
	float mSpringInnerFriction;

};

/**/
#endif //_GPhysicsComponentSpring_h_
