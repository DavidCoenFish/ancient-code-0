//
//  GPhysicsComponentLimit.h
//
//  Created by David Coen on 2010 10 30
//  Copyright 2010 Pleasure seeking morons. All rights reserved.
//

#ifndef _GPhysicsComponentLimit_h_
#define _GPhysicsComponentLimit_h_

class GVector3Float;
/*
	limit one value to be within distance of another value
*/
class GPhysicsComponentLimit
{
	//constructors
public:
	GPhysicsComponentLimit(
		const float in_maxDistance = 0.0F
		);
	GPhysicsComponentLimit(const GPhysicsComponentLimit& in_src);
	~GPhysicsComponentLimit();
	
	//operators
public:
	const GPhysicsComponentLimit& operator=(const GPhysicsComponentLimit& in_rhs);
	
	//public methods
public:
	//return true if target value was changed by limit
	const bool CalculateLimit(
		float& inout_currentValue,
		const float in_targetValue
		)const;

	//return true if target value was changed by limit
	const bool CalculateLimit(
		GVector3Float& inout_currentValue,
		const GVector3Float& in_targetValue
		)const;

	//return true if target value was changed by limit
	const bool CalculateLimit(
		GVector3Float& out_newPosition,
		GVector3Float& out_newVelocity,
		const GVector3Float& in_position,
		const GVector3Float& in_velocity,
		const GVector3Float& in_origin,
		const float in_timeDelta
		)const;

	//private members
private:
	float mMaxDistance;

};

/**/
#endif //_GPhysicsComponentLimit_h_
